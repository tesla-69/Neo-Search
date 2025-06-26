import subprocess

def run_cpp_search(query: str):
    try:
        result = subprocess.run(
            ["../cpp/NEO", query],
            capture_output=True, text=True, timeout=35
        )
        lines = result.stdout.strip().split('\n')

        docs = []
        expanded_terms = []

        for line in lines:
            if line.startswith("@@SYNONYM:"):
                # ✅ Extract expanded terms
                expanded_terms.append(line.replace("@@SYNONYM:", "").strip())
            elif "|" in line:
                parts = line.strip().split("|", 3)
                if len(parts) == 4:
                    doc_id, score, title, snippet = parts
                    docs.append({
                        "doc_id": doc_id,
                        "score": float(score),
                        "title": title.strip(),
                        "snippet": snippet.strip()
                    })

        return {
            "results": docs,
            "expanded_terms": expanded_terms  # ✅ renamed from synonyms
        }

    except Exception as e:
        return {
            "results": [],
            "expanded_terms": [],
            "error": str(e)
        }
