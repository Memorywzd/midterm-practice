from fastapi import APIRouter, HTTPException

router = APIRouter()

fake_db_domainType = [
    {"domain": "cn", "domaincompany": "ccc company",
     "domainType": "Secdomainname"},
]


@router.get("/domainType")
async def get_domain_type(domain: str, domaincompany: str):
    return fake_db_domainType
