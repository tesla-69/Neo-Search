import { useState } from "react";

export default function ResultsList({ results, expandedTerms }) {
  const [expandedDoc, setExpandedDoc] = useState(null);
  const [fullText, setFullText] = useState("");

  const handleViewFull = async (docId) => {
    if (expandedDoc === docId) {
      setExpandedDoc(null);
      setFullText("");
      return;
    }

    try {
      const res = await fetch(`/document/${docId}`);
      const data = await res.json();
      setExpandedDoc(docId);
      setFullText(data.content);
    } catch (err) {
      setFullText("Error loading document.");
    }
  };

  return (
    <div className="mt-4 w-2/3 mx-auto">
      {expandedTerms && expandedTerms.length > 0 && (
        <div>
          <p><strong>🔁 Synonyms used in search:</strong></p>
          <ul>
            {expandedTerms.map((term, idx) => (
              <li key={idx}>- {term}</li>
            ))}
          </ul>
          <hr />
        </div>
      )}

      {results.length === 0 ? (
        <p>No results yet.</p>
      ) : (
        results.map((res, idx) => (
          <div key={idx}>
            <p><strong>Doc #{res.doc_id}</strong> | Score: {res.score.toFixed(4)}</p>
            <p><strong>Title:</strong> {res.title}</p>
            <p dangerouslySetInnerHTML={{ __html: res.snippet }}></p>
            <button onClick={() => handleViewFull(res.doc_id)}>
              {expandedDoc === res.doc_id ? "Hide Full Document" : "View Full Document"}
            </button>
            {expandedDoc === res.doc_id && <pre>{fullText}</pre>}
            <hr />
          </div>
        ))
      )}
    </div>
  );
}
