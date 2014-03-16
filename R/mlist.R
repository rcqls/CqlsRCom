## goal : structure with keyname of the form "key1=val1|key2=val2|..."

mlist<-function(...) {
  keynames<-as.character(list(...))
  ml<-list()
  attr(ml,"keys")<-sort(keynames)
  class(ml)<-'mlist'
  ml
}

print.mlist<-function(ml) {
  oldClass(ml)<-'list'
  print(ml)
  class(ml)<-'mlist'
  invisible(ml)
}

"[[<-.mlist"<-function(ml,...,value) {
  params<-list(...)
   if(all(sapply(params,is.numeric))) {
    oldClass(ml)<-'list'
    ml[[...]]<-value # "[[<-"(ml,...,value) does not work!!!!
    class(ml)<-'mlist'
    ml
  } else {
    keys<-as.character(names(params))
    params<-params[o<-order(keys)]
    keys<-keys[o]
    okeys<-attr(ml,"keys")
    if(!identical(keys,okeys)) stop("bad keynames!!!")
    keyname<-paste(paste(keys,"=",params,"|",sep=""),collapse="")
    oldClass(ml)<-'list'
    ml[[keyname]]<-value
    class(ml)<-'mlist'
    ml
  }
}

as.list.mlist<-function(ml) {
  oldClass(ml)<-'list'
  ml
}

"[[.mlist"<-function(ml,...) {
  params<-list(...)
  if(all(sapply(params,is.numeric))) {
    oldClass(ml)<-'list'
    "[["(ml,...)
  } else {
    keys<-as.character(names(params))
    params<-params[o<-order(keys)]
    keys<-keys[o]
    okeys<-attr(ml,"keys")
    if(!all(keys %in% okeys)) stop("bad keynames!!!")
    keyname<-paste(paste(keys,"=",params,"\\|.*",sep=""),collapse="")
    #print(keyname)
    #print(grep(keyname,names(obj$values)))
    oldClass(ml)<-'list'
    res<-ml[grep(keyname,names(ml))]
    if(!length(res)) return(NULL) 
    if(length(res)>1) warning("Only the first element if returned")
    res<-res[[1]]
    res
  }
 }

"[.mlist"<-function(ml,...) {
  params<-list(...)
  keys<-as.character(names(params))
  params<-params[o<-order(keys)]
  keys<-keys[o]
  okeys<-attr(ml,"keys")
  if(!all(keys %in% okeys)) stop("bad keynames!!!")
  keyname<-paste(paste(keys,"=",params,"\\|.*",sep=""),collapse="")
  #print(keyname)
  #print(grep(keyname,names(obj$values)))
  oldClass(ml)<-'list'
  res<-ml[grep(keyname,names(ml))]
  class(res)<-'mlist'
  attr(res,"keys")<-okeys
  res
}

mlistKey<-function(...) {
  params<-list(...)
  if(length(params)==1 && is.list(params[[1]])) params<-params[[1]]
  keys<-as.character(names(params))
  params<-params[o<-order(keys)]
  keys<-keys[o]
  keyname<-paste(paste(keys,"=",params,"|",sep=""),collapse="")
  keyname
}

mlistParams<-function(key) {
  keys<-strsplit(grep(".*=.*",unlist(strsplit(key,"\\|")),value=TRUE),"=")
  namesRes<-sapply(keys,function(e) e[[1]])
  res<-lapply(keys,function(e) {
    if(!is.na(ee<-as.logical(e[[2]]))) ee
    else if(!is.na(ee<-suppressWarnings(as.numeric(e[[2]])))) ee
    else e[[2]]
  })
  names(res)<-namesRes
  res
}

has.key<-function(ml,...) UseMethod("has.key")
has.key.mlist<-function(ml,...) {
  keyname<-mlistKey(...)
  oldClass(ml)<-'list'
  !is.null(ml[[keyname]])
}

each<-function(obj,...) UseMethod("each")
keys<-function(obj,...) UseMethod("keys")

## The form of func is :
## func<-function(value,...)
## func<-function(value,key1=,key2=,...) ...
each.mlist<-function(ml,func,...) {
  func<-match.fun(func)
  paramFunc<-list(...)
  paramFunc<-paramFunc[setdiff(names(paramFunc),attr(ml,"keys"))]
  res<-lapply(names(ml),function(key) {
    paramList<-mlistParams(key)
    value<-unlist(do.call("[[.mlist",c(list(ml),paramList)))
    names(value)<-NULL
    do.call("func",c(list(value),paramList[intersect(formalArgs(func),names(paramList))],paramFunc))
  })
  if(is.vector(res)) names(res)<-names(ml)
  res
}

#matrix where rows correspond to the keys of the mlist
keys.mlist<-function(ml) {
  sapply(lapply(strsplit(names(ml),"\\|"),function(ch) unlist(strsplit(ch,"="))),function(e) e[c(FALSE,TRUE)])
}