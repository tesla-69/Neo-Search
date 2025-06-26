from fastapi import FastAPI, Query
from engine_wrapper import run_cpp_search  # Make sure it returns {"results": [...], "expanded_terms": [...]}
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import JSONResponse

app = FastAPI()

# ✅ CORS settings to allow frontend access
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173"],  # Adjust if needed
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
def root():
    return {"message": "NeoSearch API is running 🚀"}

# ✅ Search endpoint: returns results + expanded_terms (synonyms)
@app.get("/search")
def search(q: str = Query(..., description="Search query")):
    response = run_cpp_search(q)
    return {
        "query": q,
        "results": response.get("results", []),
        "expanded_terms": response.get("expanded_terms", [])  # ✅ Use correct key
    }

# ✅ Optional: Endpoint to fetch full document text by doc ID
@app.get("/document/{doc_id}")
def get_full_document(doc_id: int):
    try:
        with open(f"documents/doc_{doc_id}.txt", "r") as f:
            content = f.read()
        return {"doc_id": doc_id, "content": content}
    except Exception as e:
        return JSONResponse(status_code=404, content={"error": str(e)})
