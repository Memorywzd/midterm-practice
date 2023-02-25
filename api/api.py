from fastapi import APIRouter

from api import analyse, whois

api_router = APIRouter()
api_router.include_router(analyse.router, prefix="/analyse", tags=["analyse"])
api_router.include_router(whois.router, prefix="/whois", tags=["whois"])
