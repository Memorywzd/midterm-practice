from fastapi import APIRouter, HTTPException

router = APIRouter()


@router.get("/userInfo/{user_id}")
async def read_user(user_id: int):
    return {"user_id": user_id}
