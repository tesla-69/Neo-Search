import { useState } from "react";
import axios from "axios";

export default function SearchBox({ setResults, setLoading, setExpandedTerms }) {
  const [query, setQuery] = useState("");

  const handleSearch = async () => {
    if (!query.trim()) return;

    setLoading(true);

    try {
      const res = await axios.get(`http://localhost:8000/search?q=${query}`);
      setResults(res.data.results || []);
      if (setExpandedTerms) setExpandedTerms(res.data.expanded_terms || []);
    } catch (error) {
      console.error("Error searching:", error);
      setResults([]);
      if (setExpandedTerms) setExpandedTerms([]);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div>
      <input
        type="text"
        placeholder="Search..."
        value={query}
        onChange={(e) => setQuery(e.target.value)}
        onKeyDown={(e) => e.key === "Enter" && handleSearch()}
      />
      <button onClick={handleSearch}>Search</button>
    </div>
  );
}
