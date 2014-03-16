multi.data.frame<-function(...) {
  obj<-list(...)
#same number of rows or elements!!!
  if(length(obj)) 
    l<-max(sapply(obj,NROW)) #not nrow!
  else l <-0
  obj<-lapply(obj,function(e) if(is.matrix(e)) e[rep(1:NROW(e),length=l),] else e[rep(1:NROW(e),length=l)])
  class(obj)<-"multi.data.frame"
  obj
}

"[.multi.data.frame"<-function(obj,i,j) {
  class(obj)<-"list"
  if(nargs()<3) {#behave like list!!!
    obj<-obj[i]
    class(obj)<-"multi.data.frame"
    return(obj)
  }
  #otherwise like data.frame with multivariate component
  if(!missing(i)) obj<-lapply(obj,function(e) if(is.matrix(e)) e[i,] else e[i])
  if(!missing(j)) obj<-obj[j]
  class(obj)<-"multi.data.frame"
  obj
}

"[[.multi.data.frame"<-function(obj,i,j) {
  if(nargs()<3) {#behave like list!!!
    class(obj)<-"list"
    return(obj[[i]])
  }
  #otherwise like data.frame
  as.data.frame(obj[i,j])
}


dim.multi.data.frame<-function(obj) c(NROW(obj[[1]]),length(obj))

print.multi.data.frame<-function(obj,...) print(c(obj))

as.data.frame.multi.data.frame<-function(obj,...) {
  as.data.frame(c(obj),...)
}