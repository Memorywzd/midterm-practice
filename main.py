import uvicorn
from fastapi import FastAPI
from starlette.middleware.cors import CORSMiddleware

from api.api import api_router

app = FastAPI(
    title="Midterm FastAPI",
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(api_router)

if __name__ == "__main__":
    uvicorn.run(app='main:app', host="0.0.0.0", port=80, reload=True)
