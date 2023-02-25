from fastapi import APIRouter, HTTPException

router = APIRouter()

fake_db_request = [
    {"srcip": "1.1.1.1", "destip": "2.2.2.2",
     "Framelength": "2", "Accesstype": "1", "interest": "type"},
    {"srcip": "2.2.2.2", "destip": "1.1.1.1",
     "Framelength": "2", "Accesstype": "1", "interest": "type"},
]
fake_db_response = [
    {"srcip": "1.1.1.1", "destip": "2.2.2.2",
     "frameLength": "2", "content_Type": "1", "loadContent": "..."},
    {"srcip": "3.3.3.3", "destip": "4.4.4.4",
     "frameLength": "2", "content_Type": "1", "loadContent": "..."},
]


@router.get("/requestFeature")
async def get_request():
    return fake_db_request


@router.get("/responseFeature")
async def get_response():
    return fake_db_response
