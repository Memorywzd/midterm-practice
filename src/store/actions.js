//调用mutations
export const setUser = ({commit},user) => {
    commit("userStatus",user);
}
export const setToken = ({commit},token) =>{
    commit("setToken",token);
}
export const setHost = ({commit},host) =>{
    commit("setHost",host)
}
export const setContent = ({commit},content)=>{
    commit("setContent",content)
}