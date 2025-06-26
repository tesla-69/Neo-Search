import { useState } from "react";
import SearchBox from "./components/SearchBox";
import ResultsList from "./components/ResultsList";

function App() {
  const [results, setResults] = useState([]);
  const [expandedTerms, setExpandedTerms] = useState([]); // ✅ new state
  const [loading, setLoading] = useState(false);

  return (
    <div>
      <h1>NeoSearch 🔍</h1>

      <SearchBox
        setResults={setResults}
        setExpandedTerms={setExpandedTerms} // ✅ pass to SearchBox
        setLoading={setLoading}
      />

      {loading ? (
        <p>Loading...</p>
      ) : (
        <ResultsList results={results} expandedTerms={expandedTerms} /> // ✅ pass to ResultsList
      )}
    </div>
  );
}

export default App;
