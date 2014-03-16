## class GetAttr -> maybe to put in some more general package!!!
## ATTENTION: not a list!
"$.GetAttr"<-function(obj,name) attr(obj,name)
"$<-.GetAttr"<-function(obj,key,value) {
  attr(obj,key)<-value
  obj
}
"[.GetAttr"<-function(obj,...) attributes(obj)[...]

names.GetAttr<-function(obj) names(attributes(obj))
length.GetAttr<-function(obj) length(attributes(obj))


## Place this tool in another package (cqlstools Bundle containing GetAttr, mlist and Object)
## allow to save an object modified inside a R function!
## call has to be match.call() !!!
#save.Object<-function(call=match.call(),name="self") {
#  calls<-as.list(call)
#  assign(as.character(calls[[name]]),eval.parent(parse(text=name)),inherits=TRUE)
#}

## call has to be match.call() !!!
#name.Object<-function(call=match.call(),name="self") {
#  calls<-as.list(call)
#  calls[[name]]
#}


## functions to extract callname and argument names 
call.name<-function() deparse(as.list(sys.call(sys.parent()))[[1]])

object.name<-function(pos=1) {
  syscall <- as.list(sys.call(sys.parent())) 
  sapply(pos,function(i) deparse(syscall[[1+i]]))
}
