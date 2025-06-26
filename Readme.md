# NEO-SEARCH

NEO-SEARCH is a full-stack, high-performance search engine project. It combines a C++ backend for fast document indexing and search, a Python FastAPI server for API access, and a modern React frontend for user interaction.

## Features

- **C++ Search Engine:** Efficient document indexing, search, and synonym expansion
- **FastAPI Backend:** Simple REST API for search and document retrieval
- **React Frontend:** Clean, modern UI for searching and viewing results
- **Synonym Expansion:** Returns expanded search terms for improved results
- **Easy Local Development:** CORS enabled, hot-reload for backend and frontend

## Project Structure

```
NEO-SEARCH-main/
├── backend/
│   ├── api/                    # FastAPI backend (Python)
│   │   ├── app.py
│   │   ├── engine_wrapper.py
│   │   └── requirements.txt
│   └── cpp/                    # C++ search/index engine
│       ├── main.cpp
│       ├── Makefile
│       └── ...
├── frontend/                   # React + Vite frontend
│   ├── src/
│   ├── public/
│   └── package.json
└── documents/                  # Text documents to be indexed
```

## Quickstart

### 1. Build the C++ Engine

**On Windows (with MinGW):**
```bash
cd backend/cpp
mingw32-make
```

**On Linux/macOS (with make):**
```bash
cd backend/cpp
make
```

### 2. Start the FastAPI Backend

```bash
cd ../api
pip install -r requirements.txt
uvicorn app:app --reload
```

API runs at `http://localhost:8000`

### 3. Start the React Frontend

```bash
cd frontend
npm install
npm run dev
```

Frontend runs at `http://localhost:5173`

## API Endpoints

- **`GET /`** - Health check
- **`GET /search?q=your+query`** - Search for documents. Returns results and expanded terms
- **`GET /document/{doc_id}`** - Fetch the full text of a document by its ID

## Development Notes

- The FastAPI backend calls the C++ engine via `engine_wrapper.py`
- CORS is enabled for local frontend development
- Place your text documents in the `documents/` folder as `doc_{id}.txt`

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.