## Everything here is inspired from the Rcpp package!!!

## make sure system.file returns an absolute path
CqlsRCom.system.file <- function(...){
    tools:::file_path_as_absolute( base:::system.file( ..., package = "CqlsRCom" ) )
}

staticLinking <- function() {
    ! grepl( "^linux", R.version$os )
}

 
CqlsRComLdPath <- function() {
    if (nzchar(.Platform$r_arch)) {	## eg amd64, ia64, mips
        path <- CqlsRCom.system.file("lib",.Platform$r_arch)
    } else {
        path <- CqlsRCom.system.file("lib")
    }
    path
}

CqlsRComLdFlags <- function(static=staticLinking()) {
    CqlsRComDir <- CqlsRComLdPath()
    if (static) {                               # static is default on Windows and OS X
        flags <- paste(CqlsRComDir, "/libCqlsRCom.a", sep="")
        #if (.Platform$OS.type=="windows") {
        #    flags <- shQuote(flags)
        #}
    } else {					# else for dynamic linking
        flags <- paste("-L", CqlsRComDir, " -lCqlsRCom", sep="") # baseline setting
        if ((.Platform$OS.type == "unix") &&    # on Linux, we can use rpath to encode path
            (length(grep("^linux",R.version$os)))) {
            flags <- paste(flags, " -Wl,-rpath,", CqlsRComDir, sep="")
        }
    }
    invisible(flags)
}

## Provide compiler flags -- i.e. -I/path/to/CqlsRCom.h
CqlsRComCFlags <- function() {
    # path <- CqlsRComLdPath()
    path <- CqlsRCom.system.file( "include" )
    #if (.Platform$OS.type=="windows") {
    #    path <- shQuote(path)
    #}
    paste("-I", path, sep="")
}

CFlags <- function() {
    cat(CqlsRComCFlags())
}

LdFlags <- function(static=staticLinking()) {
    cat(CqlsRComLdFlags(static=static))
}


