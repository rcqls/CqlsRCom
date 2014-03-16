## a vector converter to list
# either a named vector or the list of named variables
# Vector2ListConverter(Single=1,Th=2,Th3=3) -> c2l
# or
# Vector2ListConverter(c(Single=1,Th=2,Th3=3)) -> c2l
Vector2ListConverter <- function(.list,...) {
	if(missing(.list)) .list<-list(...)
	obj <- unlist(.list)
	class(obj) <- "Vector2ListConverter"
	obj
}

by.Vector2ListConverter <- function(obj,...,envir=NULL) {
	tmp <- list(...)
	if(length(tmp)==1) vect <- tmp[[1]] else vect <- unlist(tmp)
	# cat("obj=");print(obj);print(vect);print(length(vect));print(sum(obj))
	if(length(vect)!=sum(obj)) warning("Lengths differ!")
	deb <- cumsum(c(0,unclass(obj)[-length(obj)]))+1
	fin <- deb - 1 + unclass(obj) #VERY important to unclass: since otherwise "fin" was of class Vector2ListConverter and fin[i] seems to use by method
	# cat("deb=");print(deb);cat("fin=");print(fin);print(vect[deb[1]:fin[1]])
	res <- lapply(1:length(obj),function(i) vect[deb[i]:fin[i]])
	names(res) <- names(obj)
	if(!is.null(envir)) for(var in names(res)) assign(var,res[[var]],envir=envir)
	res
}

"[.Vector2ListConverter" <- function(obj,key) by(obj,key)


## Numeric named vector easily convertible to list
# Names can be the same but in sucha case extraction has to be done with integer only!!!
NamedVector <- function(.vect,...) {
	if(missing(.vect)) .vect<-list(...)
	obj <- unlist(.vect)
	attr(obj,"var.names") <- names(.vect)
	attr(obj,"var.lengths") <- sapply(.vect,length)
	names(obj) <- paste(rep(attr(obj,"var.names"),attr(obj,"var.lengths")),unlist(sapply(attr(obj,"var.lengths"),function(e) if(e>1) paste(".",1:e,sep="") else "")),sep="")
	class(obj) <- "NamedVector"
	obj
}

"[[.NamedVector" <- function(obj,key) {
	obj <- unclass(obj)
	if(is.numeric(key)) {
		return(obj[key])
	}
	get.elt.names <- function(var) {
		if(var %in% attr(obj,"var.names")) paste(rep(var,attr(obj,"var.lengths")[[var]]),if(attr(obj,"var.lengths")[[var]]>1) paste(".",1:attr(obj,"var.lengths")[[var]],sep="") else "",sep="")
		else if(var %in% names(obj)) var
	}
	obj[unlist(sapply(key,get.elt.names))]
}

"[.NamedVector"  <- function(obj,key) {
	# transform as a list and then make a NamedVector 
	NamedVector(as.list(obj)[key])
}

names.NamedVector <- function(obj) attr(obj,"var.names")

length.NamedVector <- function(obj) attr(obj,"var.lengths")

seq.NamedVector <- function(obj) 1:length(length(obj))

as.list.NamedVector <- function(obj) {
	# transform as a list
	res <- apply(cbind(c(0,cumsum(length(obj)[1:(length(length(obj))-1)])),length(obj)),1,function(i,v) v[i[1]+1:i[2]],v=as.vector(obj))
	# add names
	names(res) <- attr(obj,"var.names")
	res
}

print.NamedVector <- function(obj,...) {
	obj<- unclass(obj)
	attr(obj,"var.names") <- NULL
	attr(obj,'var.lengths') <- NULL
	print.default(obj)
	obj
}
