#
# R Functions to interface  with snim, and with the R package easyABC
#

#'
#'  Function to run a snim model, it uses a global variable snimBin to execute the snim binary 
#'  
#'  @param simfname file name of the simulation parameters  
#'  @param parfname file name of the model parameters  
#'  @param outfname file name of the output file   
#'  
# 
run_snim <-function(simfname,parfname,outfname,spnames=""){
  if(!exists("snimBin")) stop("Variable snimBin not set")
  
  system(paste(snimBin,simfname,parfname,outfname))
  
  if(file.exists(outfname))
  require(tidyr)
  require(dplyr)
  out <- read.table(outfname)
  if(spnames[1]!="") {
    require(vegan)
    spnames <- c("0",make.cepnames(spnames))
    out <- out  %>% mutate(Species=spnames) 
  } else {
    out <- out  %>% mutate(Species=as.character(c(0:(n()-1))))   
  }
  out <- out  %>% gather(key = Time, value = Density, -Species) %>% mutate(Time=as.numeric(substring(Time,2)))

 return(out) 
}


#'
#'  Function to run and plot species of snim model, it uses a global variable snimBin to execute the snim binary  
#'  
#'  
#'  @param simfname file name of the simulation parameters  
#'  @param parfname file name of the model parameters  
#'  @param outfname file name of the output file   
#'  
# 
run_snim_plot <-function(simfname,parfname,outfname,spnames=""){
  if(!exists("snimBin")) stop("Variable snimBin not set")
  
  system(paste(snimBin,simfname,parfname,outfname))
  
  if(file.exists(outfname))
    require(tidyr)
  require(dplyr)
  out <- read.table(outfname)
  if(spnames[1]!="") {
    require(vegan)
    spnames <- c("0",make.cepnames(spnames))
    out <- out  %>% mutate(Species=spnames) 
  } else {
    out <- out  %>% mutate(Species=as.character(c(0:(n()-1))))   
  }
  out <- out  %>% gather(key = Time, value = Density, -Species) %>% mutate(Time=as.numeric(substring(Time,2)))
  require(ggplot2)
  halfTime <- max(out$Time)/4
  out1 <-filter(out,Species!="0", Time>halfTime & Density>0) 
  out1 <-filter(out,Species %in% unique(out1$Species))
  print(ggplot(out1, aes(x = Time, y = Density,colour=Species)) + geom_line() +  theme_bw() + scale_y_log10()) 
  
  return(out) 
}


#'
#'  Calculate average of species from a particular time 
#'  
#'  @param da data.frame with species   
#'  @param time time from which to calculate the average 
#'  
# 
calc_avg_fromtime <-function(da, ftime=0)
{
  da <- da %>% filter(Time>=ftime) %>% group_by(Species) %>% summarize(Density=mean(Density))
  return(da)
}

#'
#'  Generate a parameter file to run SNIM model  
#'  
#'  @param parfname Name of the parameter file to generate    
#'  @param nsp Number of species of the metacommunity
#'  @param totCom Total number of individuals in the community
#'  @param m Migration vector 
#'  @param e Extinction vector 
#'  @param omega Interaction matrix  
#'   
# 
generate_parfile <- function(parfname, nsp,totCom,m,e,omega)
{
  sink(parfname)
  cat(paste("# model parameters ","\n"))
  cat(paste("# numSp totalCommunity","\n"))
  cat(paste(nsp,totCom))
  nsp <- nsp+1
  cat(paste("\n# Immigration vector","\n"))
  cat(m); 
  cat(paste("\n# Extinction vector","\n"))
  cat(e); 
  cat(paste("\n# Omega","\n"))
  for(i in 1:nsp){ 
    cat(omega[i,]) 
    cat("\n")
  }
  sink()
}


#'
#'  Read a SNIM model parameter file   
#'  
#'  @param parfname Name of the parameter file to generate    
#'  @param nsp Number of species of the metacommunity
#'  @param totCom Total number of individuals in the community
#'  @param m Migration vector 
#'  @param e Extinction vector 
#'  @param omega Interaction matrix  
#'   
# 
read_parfile <- function(parfname)
{
  pl <-readLines(parfname)
  pl <- pl[!grepl("^#",pl)]
  l1 <- read.table(textConnection(pl[1]))
  nsp <- l1[1,1]
  totCom <-l1[1,2]
  
  l2 <- read.table(textConnection(pl[2]))
  m <- as.numeric(l2[1,])
  l3 <- read.table(textConnection(pl[3]))
  e <- as.numeric(l3[1,])
  l4 <- read.table(textConnection(pl[4:length(pl)]))
  omega <- unname(as.matrix(l4))
  return(list(nsp=nsp,totCom=totCom,m=m,e=e,omega=omega))
}


#'
#'  Run SNIM for easyABC package only varying omega (interactions)    
#'  
#'  @param x  coeficent of interacitons of the matrix in column order
#'   
# 
snim_easyABC_omega <- function(x)
{
  if(!exists("parlist")) stop("Can't find base parameters to run model")

  # Copy x parameters to omega matrix, only interactions from row,col>1 
  k <- 1
  for(i in 2:ncol(parlist$omega))
    for(j in 2:nrow(parlist$omega))
    {
      if(parlist$omega[j,i]>0){
        parlist$omega[j,i]<-x[k]
        k<-k+1
      }
    }
  
  generate_parfile("easyABC.par",parlist$nsp,parlist$totCom,parlist$m,parlist$e,parlist$omega)
  parf <- "easyABC.par"
  simf <- "easyABC.cfg"
  outf <- "easyABC.out"
  
  out <- run_snim(simf,parf,outf)
  avg <- filter(calc_avg_fromtime(out,50),Species!=0) %>% filter(Density>0) %>% mutate(Freq=Density/parlist$totCom) %>% summarise(S=n(),H=-sum(Freq*log(Freq)))
  return(avg$S)
}

snim_genRnd_omega <- function(parlist)
{
  dc <- apply(parlist$omega,2,sum)
  gr <- apply(parlist$omega,1,sum)
  gr1 <- gr[2:(parlist$nsp+1)]- (1/parlist$e) - dc[2:(parlist$nsp+1)]
  
  # gr1 should be positive but lower than 1
  
  parlist$nsp
  # Copy x parameters to omega matrix, only interactions from row,col>1 
  k <- 1
  for(i in 2:nrow(parlist$omega))
  {
    sum(parlist$omega[2,]>0)
    for(j in 2:ncol(parlist$omega))
    {
      if(parlist$omega[j,i]>0){
        parlist$omega[j,i]<-x[k]
        k<-k+1
      }
    }
  }
}