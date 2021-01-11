C      PROGRAM MAIN
C      END
C
      subroutine hyp_event(Momentum,Nreson,Nchanel,Ngamma,Pgamma)
C                                                                                       Sdv
C     subroutine hyp_event(Ptarget,Ntarget,Nchanel,Nreson,Ngamma,Pgamma,Momentum)    17.02.2019
C     Omitted parameters:  Ptarget,Ntarget 
C 
C     Real input parameters:                   Output parameters:
C                Momentum -- beam mom GeV/c    Nchanel -- decay channel of resonance
C                Nreson   -- reaction index    Ngamma  -- number of photons in final stage
C                                              Pgamma  -- array of 10-vectors of the photons 
C
C     MC for the inclusive reaction:
C            a  +  bZN --> c0 + X
C            c0 --> c1 + c2 + ...
C........................................................................................... 
C-    IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      SAVE
      Double precision Momentum
      Dimension Ptarget(10)
      Dimension Pchanel(10,10)  ! Number of resonans, Number of chanel
      Dimension Preson(10)
      Dimension PtrgtSm(10), PchanSm(10,10), PresSm(10)
      Double precision T,Pa(5),Pb(5),Pc(5),Pd(5),Pgamma(10,10)  
      Double precision P1(5), P2(5), P3(5), P4(5), PPc, CtauK0
      Double precision Xvertex, Yvertex, Zvertex, Random, R1, R2, FF
      Double precision Double_Mass_distr_2pi0
      Integer Mchanel(10)    
C       
      Integer ExTarget, ExReson, ExChanel
C-    data    ExTarget, ExReson, ExChanel  / 3, 4, 1/ !  The main parameters for f2 exclusive channel 
      data    ExTarget, ExReson, ExChanel  / 3, 6, 1/ !  The main parameters for omg exclusive channel
      data Mtarget,Mgamma,Mreson  / 3, 10, 7 /        !  3 targets, 10 photons maximum, 7 resonanses (yet) 
C     data Ptarget  /  1. 0, 1.0, 1.0, 0.0, 6*0. /    !  Target probability
C     data Preson   /  0.14124, 0.02401, 0.03107, 0.05367, 
C     +                         0.74576, 0.00423, 0.057, 3*0./   !  Resonans probability
      data Preson   /  0.041, 0.024, 0.031, 0.054,
     +                           0., 0.032, 0.057,  3*0./        !  Resonans probability
C
      data Mchanel  /     1,   3,   1,   1,   1,  1,  1, 3*0 /   !  Decay cahannels
      data Pchanel  /  1.00, 9*0.0,                              !  pi0 /pi0->2Y/
     2                39.31, 32.57, 0.027,  7*0.0,               !  eta /eta->2Y, eta->3pi0->6Y, eta->2Ypi0->4Y/
     3                30.69, 9*0.0,                              !  K0 /K0->2pi0->4Y/
     4                28.27, 9*0.0,                              !  f2 /f2->2pi0->4Y/
     5                1.000, 9*0.0,                              !  2pi0 /2pi0->4Y/
     6                1.000, 9*0.0,                              !  w /w->pi0Y->3Y/
     7                1.000, 9*0.0,                              !  f0 /f0->2pi0->4Y/
     8                                      30*0.0  /            !  All other
      data CtauK0     / 2.404  /                                 !  C*lifetime(K0s)
C ..............................................................................
      Logical Lstart
      data Lstart   /.false./      
      data    Ampi,    Ampi0,      AmK,     AmK0,      Amp,      Amn    
     +/   0.139570, 0.134977, 0.493677, 0.497611, 0.938272, 0.939565 /
      data   AmEta,    AmKsi,     Amf2,     Amw      
     +/   0.547862,  1.18937,   1.2755,  0.78265 / 
      data    s2f2,      s2w/   0.1851,  0.00849 / ! Widths of f2 and omg
C-------------------------------------------------------------------------------   
C
      if(Nreson.gt.Mreson) then
                   write(*,*) 'hyp_carlo: Ireact=',Nreson
		   write(*,*) '************************************************'
                   stop 'hyp_carlo: ERROR reaction Index'
	       endif
C
C-    if(Lstart) go to 10           !  Riginal operator for primary hyp_carlo routine
      if(Lstart) go to 40           !  Operator for call from Hyp_RegGen routine
C....................................................................................
C Target randomizer 
C
C-    do j=1,Ntarget
C-    R1=RNDM(-1.)
C-    enddo
C 
C Target probablities:
C
C-    CrSec = 0.
C-    do j=1,Mtarget
C-    CrSec = CrSec + Ptarget(j)
C-    enddo
C
C-    PtrgtSm(1)= Ptarget(1)/CrSec
C-    do j=1,Mtarget-1
C-    PtrgtSm(j+1) = PtrgtSm(j)+Ptarget(j+1)/CrSec
C-    enddo	 
C-    if(PtrgtSm(Mtarget).lt.0.9999) stop'Carlo: Target probability ERR'
C-    if(ExTarget.gt.Mtarget)	     stop'Carlo: Exclusive target ERR'
C
C-    if (ExTarget.gt.0) then
C-      		 Ntarget = ExTarget
C-      		 write(*,*) 'Carlo: Eclusive target number =',Ntarget
C-      else		       
C-      do j=1,Mtarget
C-      write(*,*)'Carlo: Target_Nb, Probability=',j,Ptarget(j)/CrSec
C-      enddo
C-    endif
C
C Resonans probablities:
C
C-    CrSec = 0.
C-    do j=1,Mreson
C-    CrSec = CrSec + Preson(j)
C-    enddo
C
C-    PresSm(1)= Preson(1)/CrSec
C-    do j=1,Mreson-1
C-    PresSm(j+1) = PresSm(j)+Preson(j+1)/CrSec
C-    enddo	 
C-    if(PresSm(Mreson).lt.0.9999)stop'Carlo: Resonance probability ERR'
C-    if(ExReson.gt.Mreson)	stop'Carlo: Exclusive resonance ERR'
C
C-    if (ExReson.gt.0) then
C-   		  Nreson = ExReson
C-   		  write(*,*) 'Carlo: Eclusive resonance number =',Nreson
C-      else		       
C-      do j=1,Mreson
C-      write(*,*)'Carlo: Resonance Nb Probability:',j,Preson(j)/CrSec
C-      enddo
C-    endif
C.................................................................................
C
C Exclusive channel decay probabilities 
C
C1-   if (ExChanel.le.0) then
       do n=1,Mreson
        CrSec = 0.
        do j=1,Mchanel(n)
        CrSec = CrSec + Pchanel(j,n)
        enddo
C
        PchanSm(1,n)= Pchanel(1,n)/CrSec
        do j=1,Mchanel(n)-1
        PchanSm(j+1,n) = PchanSm(j,n)+Pchanel(j+1,n)/CrSec
        enddo      
        if (PchanSm(Mchanel(n),n).lt.0.9999) then
             stop 'Channel prob. error'   
	  else  		 
	   do j=1,Mchanel(n)
	    write(*,*)'Res. Nb, Channel Nb, Problt:',n,j,Pchanel(j,n)/CrSec
	   enddo
        endif
       enddo     
C
C1-   else
C1-   Nchanel = ExChanel
C1-     write(*,*) 'Exclusive decay channel number =',Nchanel
C1-     
C1-     if(ExReson.le.0) then
C1-      do j=1,Mreson  
C1-       if(ExChanel.gt.Mchanel(j)) stop 'Excl.decay channel error'
C1-      enddo
C1-     else
C1-      if(ExChanel.gt.Mchanel(ExReson)) stop 'Excl.decay chnl error'
C1-     endif
C1-   endif 
C 
C Beam:
C
      Pbeam = Momentum
      Pa(5) = Ampi      
      Pa(1) = 0.
      Pa(2) = 0.
      Pa(3) = Pbeam
      Pa(4) = Dsqrt(Pa(5)**2 + Pa(3)**2 + Pa(2)**2 + Pa(1)**2)
C
C Target:
C
      Pb(5) = Amn     
      Pb(1) = 0.
      Pb(2) = 0.
      Pb(3) = 0.
      Pb(4) = Dsqrt(Pb(5)**2 + Pb(3)**2 + Pb(2)**2 + Pb(1)**2) 	
C
C Photon masses:       
C
      do j=1,Mgamma
         Pgamma(5,j) = 0.
      enddo
C      
      Lstart=.true.
C     read(*,*)
C --------------------      
C       
C Choose target:
C
C- 10 if (ExTarget.le.0) then 
C-    R1 = RNDM(-1.)
C-    do j=1,Mtarget
C-    if (R1.lt.PtrgtSm(j)) then
C-			    Ntarget = j     
C-			    go to 20
C-		     endif 
C-	     enddo
C-	else 
C-    Ntarget = ExTarget
C-    endif
C
C Choose resonans:
C
C- 20 if(ExReson.le.0) then
C-    R1 = RNDM(-1.)
C-    do j=1,Mreson
C-    if (R1.lt.PresSm(j)) then
C- 			   Nreson = j	  
C- 			   go to 30
C- 		     endif 
C- 	  enddo
C- 	 else
C-     Nreson = ExReson
C-     endif 
C  
C Choose excl. decay channel:
C
C- 30 if (ExChanel.le.0) then
C-    R1 = RNDM(-1.)
C-    do j=1,Mchanel(Nreson)
C-    if (R1.lt.PchanSm(j,Nreson)) then
C- 			    Nchanel = j     
C- 			    go to 40
C- 		     endif 
C- 	  enddo
C- 	else
C-    Nchanel = ExChanel
C-    endif    
C
   40 continue      
C	    
C Spectator:
C
      Pd(5) = Amp     
C
C C-system:  
C             
      
      if (Nreson.eq.1) then                         !  Resonans 1 - pi0
       
        call reaction(Nreson, T, Pd(5))
        
        Pc(5) = Ampi0
	call abtocds(Pa,Pb(5),Pc,Pd,T)
	if (T.gt.0.) go to 1000
        if (Nchanel.eq.1) then                      !  Excl. channel = pi0 -> 2Y
C          write(*,*) 'Pi0->2Y'
          call decays (Pc,Pgamma(1,1),Pgamma(1,2))
          Ngamma = 2	  
        endif
C
        do j=1,Ngamma
	Pgamma(6,j) = 0.
	Pgamma(7,j) = 0.
	Pgamma(8,j) = 0.
	Pgamma(9,j) =22.                            !  PDG code of photon
	Pgamma(10,j)=100*Nreson+10*Nchanel+j        !  Photon hystory code
        enddo
      endif
C
      if (Nreson.eq.2) then	                    !  Resonans 2 - eta 
         Pc(5) = AmEta
         call reaction(Nreson, T, Pd(5))  
         call abtocds(Pa,Pb(5),Pc,Pd,T)
	 if (T.gt.0.) go to 1000
C	  
	    if (Nchanel.eq.1) then                  !  Excl. channel = eta -> 2Y
             call decays (Pc,Pgamma(1,1),Pgamma(1,2))
             Ngamma = 2
            endif
C
            if (Nchanel.eq.2) then                  !  Excl. channel = eta -> 3pi0 ->6Y
             P1(5) = Ampi0
             P2(5) = Ampi0
             P3(5) = Ampi0
             call STAR3T(Pc,P1,P2,P3)
             call decays(P1,Pgamma(1,1),Pgamma(1,2))
             call decays(P2,Pgamma(1,3),Pgamma(1,4))					     
             call decays(P3,Pgamma(1,5),Pgamma(1,6))
             Ngamma = 6
C-
C            write(*,*)
C	     write(*,*)
C
C	     do l=1,4
C	     P4(l) = 0.
C	     enddo
C
C	     do l=1,6
C	     P4(1) = P4(1)+Pgamma(1,l)
C	     P4(2) = P4(2)+Pgamma(2,l)
C	     P4(3) = P4(3)+Pgamma(3,l)
C	     P4(4) = P4(4)+Pgamma(4,l)

C	     enddo
C
C	     Esm = P4(4)
C	     call hf1(105,Esm,1.)
C-
     
C  77	     format(5E20.10)
C             do k=1,4
C	     Pc(k)= P1(k)+P2(k)+P3(k)
C             enddo
            endif

C
            if (Nchanel.eq.3) then                  !  Excl. channel = eta -> 2Ypi0 ->4Y
	    P1(5) = Ampi0
	    call STAR3T(Pc,P1,Pgamma(1,1),Pgamma(1,2))
            call decays(P1,Pgamma(1,3),Pgamma(1,4))
            Ngamma = 4
        endif
C
      	do j=1,Ngamma
        Pgamma(6,j) = 0.
	Pgamma(7,j) = 0.
	Pgamma(8,j) = 0.
	Pgamma(9,j) =22.                            !  PDG code of photon
	Pgamma(10,j)=100*Nreson+10*Nchanel+j        !  Photon hystory code
	enddo
      endif
C
      if (Nreson.eq.4) then                         !  Resonance 4 - K0s
         Pc(5) = AmK0
         call reaction(Nreson, T, Pd(5))                 
         call abtocds(Pa,Pb(5),Pc,Pd,T)
         if (T.gt.0) go to 1000
C
C         
         if (Nchanel.eq.1) then                     !  Excl. channel = K0s -> 2pi0->4Y
          P1(5) = Ampi0
          P2(5) = Ampi0
          call decays(Pc, P1, P2)
          call decays(P1, Pgamma(1,1), Pgamma(1,2))
          call decays(P2, Pgamma(1,3), Pgamma(1,4))
          Ngamma = 4
         endif
C
C
        Random = RNDM(-1.)
        PPc = dsqrt((Pc(1)**2+Pc(2)**2 + Pc(3)**2))
        Xvertex = - (Pc(1)/PPc)*(Pc(4)/Pc(5))*CtauK0*dlog(Random)
        Yvertex = - (Pc(2)/PPc)*(Pc(4)/Pc(5))*CtauK0*dlog(Random)
        Zvertex = - (Pc(3)/PPc)*(Pc(4)/Pc(5))*CtauK0*dlog(Random)
        do j=1,Ngamma
        Pgamma(6,j) = Xvertex
	Pgamma(7,j) = Yvertex
	Pgamma(8,j) = Zvertex
	Pgamma(9,j) = 22.
	enddo
      endif
C     
      if (Nreson.eq.5) then                         !  Resonance 5 - f2
C-       Pc(5) = Amf2 + (s2f2/2.36)*rnd_gauss(1.)   !  Simple Gauss
	 call BW_rand_f2(1275.1, 185.1,rnd_BW)                    !  GAMS BW for f2(1275)
	 Pc(5) = 0.001*rnd_BW                                     !  MeV => GeV
C-	 
         call reaction(Nreson, T, Pd(5))
         call abtocds(Pa,Pb(5),Pc,Pd,T)
         if (T.gt.0) go to 1000
C
         if (Nchanel.eq.1) then                     !  Excl. channel = f2 -> 2Pi0 -> 4Y
          P1(5) = Ampi0
          P2(5) = Ampi0
          call decays(Pc, P1, P2)
          call decays(P1, Pgamma(1,1), Pgamma(1,2))
          call decays(P2, Pgamma(1,3), Pgamma(1,4))
          Ngamma = 4
         endif
C
C
         do j=1,Ngamma
         Pgamma(6,j) = 0.
	 Pgamma(7,j) = 0.
	 Pgamma(8,j) = 0.
	 Pgamma(9,j) =22.                           !  PDG code of photon
	 Pgamma(10,j)=100*Nreson+10*Nchanel+j       !  Photon hystory code
         enddo
      endif
      
C
      if (Nreson.eq.6) then                         !  Resonance 6 - 2pi0
  854    R1=RNDM(-1.)
         R2=RNDM(-1.)
         FF=Double_Mass_distr_2pi0(R1*1600.)
C         write(*,*) 'Mass distr = ',FF
         if((R2*800.).gt.FF) go to 854
         Pc(5) = R1*1.6
         P1(5) = Ampi0
         P2(5) = Ampi0
         call reaction(Nreson, T, Pd(5))
         call abtocds(Pa,Pb(5),Pc,Pd,T)
         call decays(Pc, P1, P2)
         if (T.gt.0) go to 1000
C
         if (Nchanel.eq.1) then                     !  Excl. channel 2pi0
           call decays(P1, Pgamma(1,1), Pgamma(1,2))
           call decays(P2, Pgamma(1,3), Pgamma(1,4))
           Ngamma = 4
         endif
C
         do j=1,Ngamma
         Pgamma(6,j) = 0.
	 Pgamma(7,j) = 0.
	 Pgamma(8,j) = 0.
	 Pgamma(9,j) =22.                           !  PDG code of photon
	 Pgamma(10,j)=100*Nreson+10*Nchanel+j       !  Photon hystory code
         enddo
      endif
C
      if (Nreson.eq.3) then                         !  Resonance 3 - omega
C-       Pc(5) = Amw + (s2w/2.36)*rnd_gauss(1.)     !  Simple Gauss distribution
C
	 call BW_rand_omg(782.65, 8.49 ,rnd_BW)     !  GAMS BW for omg(782)
	 Pc(5) = 0.001*rnd_BW                       !  MeV => GeV	 	    
C
         call reaction(2, T, Pd(5))                 !  Just like eta
         call abtocds(Pa,Pb(5),Pc,Pd,T)
         if (T.gt.0) go to 1000
C
         if (Nchanel.eq.1) then                     !  Excl. channel = w -> pi0Y -> 3Y
          P1(5) = Ampi0
          call decays(Pc, P1, Pgamma(1,1))
          call decays(P1, Pgamma(1,2), Pgamma(1,3))
          Ngamma = 3
         endif
C         
         do j=1,Ngamma
         Pgamma(6,j) = 0.
	 Pgamma(7,j) = 0.
	 Pgamma(8,j) = 0.
	 Pgamma(9,j) =22.                           !  PDG code of photon
	 Pgamma(10,j)=100*Nreson+10*Nchanel+j       !  Photon hystory code
         enddo
      endif	 
C
      if (Nreson.eq.7) then                         !  Resonance 7 - f0(500) -> 2pi0	 
	 call BW_rand_f0(446.0, 350.0,rnd_BW)       !  GAMS BW for f0(500)
	 Pc(5) = 0.001*rnd_BW                       !  MeV => GeV
C-	 
         Mreson= 5
	 call reaction(Mreson, T, Pd(5))
         call abtocds(Pa,Pb(5),Pc,Pd,T)
         if (T.gt.0) go to 1000
C
         if (Nchanel.eq.1) then                     !  Excl. channel = f0 -> 2Pi0 -> 4Y
          P1(5) = Ampi0
          P2(5) = Ampi0
          call decays(Pc, P1, P2)
          call decays(P1, Pgamma(1,1), Pgamma(1,2))
          call decays(P2, Pgamma(1,3), Pgamma(1,4))
          Ngamma = 4
         endif
C
         do j=1,Ngamma
         Pgamma(6,j) = 0.
	 Pgamma(7,j) = 0.
	 Pgamma(8,j) = 0.
	 Pgamma(9,j) =22.                          !  PDG code of photon
	 Pgamma(10,j)=100*Nreson+10*Nchanel+j      !  Photon hystory code
         enddo
      endif
C
      return
C       
 1000 CONTINUE 
      write(*,*) '1000'
      Ngamma = 0
      RETURN
      END
C
      subroutine BW_rand_f2(Rmas, Rwid, rnd_BW)
C-    Breit-Wigner random number generator for f2:  S.Sadovsky 17.08.2011
C-    Eur.Phys.J. A3, 361-371 (1998)
C 
      save Ampi2, BWnorC, BWnorGAMS
      data Ampi2, BWnorC, BWnorGAMS /   270.0,  205.0, 0.02613 / ! BW normalizations for f2 
C-    data Rmas, Rwid        / 1275.1, 185.1   / ! f2 parameters, not used here
C
   10 call BW_rand(Rmas, Rwid, X_mas)
      if (X_mas.lt.Ampi2) go to 10                          ! 2pi0 channel threshold
      BW_class =   BWnorC   *BW_fun    (Rmas, Rwid, X_mas)    
      BW_gams  =   BWnorGAMS*BW_f2_gams(Rmas, Rwid, X_mas) 
      if (BW_class*rndm(-1.).gt.BW_gams) go to 10  
      rnd_BW   =   X_mas
      return
      end
C
      subroutine BW_rand_f0(Rmas, Rwid, rnd_BW)
C-    Breit-Wigner random number generator for f0:  S.Sadovsky 14.12.2015
C-    Eur.Phys.J. A3, 361-371 (1998)
C 
      save Ampi2, BWnorC, BWnorGAMS
      data Ampi2, BWnorC, BWnorGAMS /  270.0,  205.0, 0.02613 / ! BW normalizations for f2 
C-    data Rmas,  Rwid        / 446.0, 350.0  / ! f0 parameters, not used here
C
   10 call BW_rand(Rmas, Rwid, X_mas)
      if (X_mas.lt.Ampi2) go to 10                          ! 2pi0 channel threshold
      BW_class =   BWnorC   *BW_fun    (Rmas, Rwid, X_mas)    
      BW_gams  =   BWnorGAMS*BW_f0_gams(Rmas, Rwid, X_mas) 
      if (BW_class*rndm(-1.).gt.BW_gams) go to 10  
      rnd_BW   =   X_mas
      return
      end
C
      subroutine BW_rand(Rmas, Rwid, rnd_BW)
C-    Breit-Wigner random number generator:         S.Sadovsky 17.08.2011
      real Rmas, Rwid, rnd_BW     
      data pi / 3.141592653589 /
C
   10 rnd_BW = Rmas*Rmas + Rmas*Rwid*tan(pi*(rndm(-1.)-0.5))
      if (rnd_BW.gt.0.) then
                        rnd_BW = sqrt(rnd_BW)
	       else
	       go to 10
      endif	                   
      return
      end
C
      function BW_fun(Rmas, Rwid, X)
C     The classical relativistic BW function:  S.Sadovsky 17.08.2011
C-    data Rmas,Rwid /  1275.1, 185.1 /     !  f2 parameters in MeV, not used here
C                 
      BW_fun= X*Rmas*Rwid**2/((X*X-Rmas**2)**2+(Rmas*Rwid)**2)
      return
      end   
C
      function BW_f2_gams(Rmas, Rwid, X)
C     The GAMS relativistic BW function for f2:      S.Sadovsky 17.08.2011 
C     Eur.Phys.J. A3, 361-371 (1998)
C     Input:   X    - 2pi0 mass in MeV
C              Rmas - resonanse mass  in MeV
C              Rwid - resonanse width in MeV
C
      data Ampi0, r0  /  134.9766, 197.326      /  ! pi0 mass and r0 (in 1/MeV)
C-    data Rmas, Rwid / 1275.1, 185.1 /            ! f2 parameters in MeV, not used here
C                        					      
      ppi0  = sqrt(abs((Rmas/2.)**2-Ampi0**2))
      ppiX  = sqrt(abs((X/2.)**2   -Ampi0**2))
C     
      D2pi0 = 9.+3.*(ppi0/r0)**2+(ppi0/r0)**4
      D2piX = 9.+3.*(ppiX/r0)**2+(ppiX/r0)**4
C
      Gamma = Rwid*(ppiX/ppi0)**5*D2pi0/D2piX                  
      BW    = Rmas**2*Gamma**2/((x*x-Rmas**2)**2+(Rmas*Gamma)**2)
      BW_f2_gams = Bw*X**2/ppiX
      return
      end 
C
      function BW_f0_gams(Rmas, Rwid, X)
C     The GAMS relativistic BW function for f0:    S.Sadovsky 14.12.2015 
C     Eur.Phys.J. A3, 361-371 (1998)
C     Input:   X    - 2pi0 mass in MeV
C              Rmas - resonanse mass  in MeV
C              Rwid - resonanse width in MeV
C
      data Ampi0, r0  /  134.9766, 197.326      /  ! pi0 mass and r0 (in 1/MeV)
C-    data Rmas, Rwid /  446.0, 350.0 /            ! f0 parameters in MeV, not used here
C                        					      
      ppi0  = sqrt(abs((Rmas/2.)**2-Ampi0**2))
      ppiX  = sqrt(abs((X/2.)**2   -Ampi0**2))
C     
C-    D2pi0 = 9.+3.*(ppi0/r0)**2+(ppi0/r0)**4      ! for f2 --> not used for f0 (sigma)
C-    D2piX = 9.+3.*(ppiX/r0)**2+(ppiX/r0)**4      ! for f2 --> not used for f0 (sigma)
C
C-    Gamma = Rwid*(ppiX/ppi0)**5*D2pi0/D2piX      ! for f2 
      Gamma = Rwid*(ppiX/ppi0)                     ! for f0 (sigma)
C                 
      BW    = Rmas**2*Gamma**2/((x*x-Rmas**2)**2+(Rmas*Gamma)**2)
      BW_f0_gams = Bw*X**2/ppiX
      return
      end       
C      
      subroutine BW_rand_omg(Rmas, Rwid, rnd_BW)
C-    Breit-Wigner random number generator for omega:  S.Sadovsky 17.08.2011
C-    Eur.Phys.J. A3, 361-371 (1998)
C 
      save BWnorC, BWnorGAMS
      data BWnorC, BWnorGAMS /   1.00, 2.613E-4/ ! BW normalizations for omega
C-    data Rmas, Rwid        / 782.65, 8.49    / ! omg parameters, not used here
C
   10 call BW_rand(Rmas, Rwid, X_mas)
      BW_class =   BWnorC   *BW_fun     (Rmas, Rwid, X_mas)    
      BW_gams  =   BWnorGAMS*BW_omg_gams(Rmas, Rwid, X_mas)
      if (BW_class*rndm(-1.).gt.BW_gams) go to 10  
      rnd_BW   =   X_mas
      return
      end   
C
      function BW_omg_gams(Rmas, Rwid, X)
C     The GAMS relativistic BW function for f2:      S.Sadovsky 17.08.2011 
C     Eur.Phys.J. A3, 361-371 (1998)
C     Input:   X    - 2pi0 mass in MeV
C              Rmas - resonanse mass  in MeV
C              Rwid - resonanse width in MeV
C
      data Ampi0, r0  / 134.9766, 121.12 /        ! pi0 mass and inverse r0 for omega in MeV
C-    data Rmas, Rwid / 782.65,     8.49 /        ! omg parameters in MeV, not used here
C                        					      
      ppi0  = sqrt(abs((Rmas/2.)**2-Ampi0**2))
      ppiX  = sqrt(abs((X/2.)**2   -Ampi0**2))
C     
      D2pi0 = 1.+(ppi0/r0)**2	                   ! Blatte-Weiskopf factor for omega
      D2piX = 1.+(ppiX/r0)**2
C
      Gamma = Rwid*(ppiX/ppi0)**3*D2pi0/D2piX                  
      BW    = Rmas**2*Gamma**2/((x*x-Rmas**2)**2+(Rmas*Gamma)**2)
      BW_omg_gams = Bw*X**2/ppiX
      return
      end 
C      
      subroutine Ptrans(Pt,Pa,Pb,t)
C
C     Elastic emmission of photons in the target vertex:
C     G.Baur et al, Phys.Rep.364 (2002), p.359-450. 
C      
      double precision Pt(7),Pa(7),Pb(7),t,pi2,phi
      logical Logb
      save    Logb, Ra, Rb,gamma,Omax,Otmx,Omin,Wmax
      data r0,Logb, hc/ 1.2, .true., 0.197326 /   ! r0 [Fm], hc [GeV*Fm]
      data pi2/ 6.283185307/    
C
      if (logb) then
                Ra   = r0*(Pa(6)+Pa(7))**0.3333333333
		Rb   = r0*(Pb(6)+Pb(7))**0.3333333333
		gamma= Pa(4)/Pa(5)
		Omax = gamma*hc/(Ra+Rb) 	
		Otmx = hc/Rb
		Omin = 0.0001
		Wmax = alog(Omax/Omin)
		write(*,*) 'Et(min),Et(max),Pt(max)=',Omin,Omax,Otmx 
                Logb =.false.
          endif 
C                                             Energy of the virtual photon
   10 Omega =-Omax*alog(RNDM(-1.))
      if (Omega.lt.Omin.or.Omega.gt.Omax)     go to 10
      if (Wmax*RNDM(-1.).gt.alog(Omax/Omega)) go to 10
C      	                              
   20 qt =-Otmx*alog(RNDM(-1.))          !    Trannsverse momentum
      if (qt.gt.Otmx)    go to 20
      Omg  = Omega/gamma
      Wqmx = 1./(16.*Omg)                !    Omgmx = sqrt(3)*Omg
      if (Wqmx*RNDM(-1.).gt.qt**3/(Omg**2+qt**2)**2) go to 20
C  
      t     =-Omega**2-qt**2
      phi   = pi2*RNDM(-1.)
      Pt(1) = qt*cos(phi)
      Pt(2) = qt*sin(phi)
      Pt(3) =-Omega
      Pt(4) =-sqrt(abs(Omega**2+qt**2 + t))
      Pt(5) = t
      return
      end
C
C**************************************************************************
C
      SUBROUTINE STAR3T(PL0,PL1,PL2,PL3)
C.........................................................................
C   TO GENERATE IN CMS ISOTROPIC DECAY OF ONE PARTICLE INTO THREE ONE
C   INPUT  VALUES:   PL0(5),PL1(5),PL2(5),PL3(5) - MASSES OF PARTICLES
C   OUTPUT VALUES:   PL1(1-4),PL2(1-4),PL3(1-4) - 4-VECTORS
C
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      REAL RNDM
      DIMENSION PL0(5),PL1(5),PL2(5),PL3(5),P1(5),P2(5),P3(5)
      DATA PI2 / 6.28318530718 /
C
      T0 = PL0(5) - PL1(5) - PL2(5) - PL3(5)
C
   10 R1 = RNDM(-1.)
      R2 = RNDM(-1.)
      IF  (R1.LT.R2) GO TO 20
      R1 = 1. - R1
      R2 = 1. - R2
C
   20 T1 = T0* R1
      T2 = T0*(R2-R1)
      T3 = T0- T1-T2
c
      EE1 = PL1(5) + T1
      EE2 = PL2(5) + T2
      PP1 = DSQRT( T1*(T1 + 2.*PL1(5)) )
      PP2 = DSQRT( T2*(T2 + 2.*PL2(5)) )
      PP3 = DSQRT( T3*(T3 + 2.*PL3(5)) )
C
      IF(PP1.GT.PP2+PP3) GO TO 10
      IF(PP2.GT.PP3+PP1) GO TO 10
      IF(PP3.GT.PP1+PP2) GO TO 10
C
      C1  = 2.*RNDM(-1.) - 1.
      S1  = DSQRT( 1.-C1*C1 )
      F1  = PI2*RNDM(-1.)
      CF1 = DCOS(F1)
      SF1 = DSIN(F1)
      P1(1) = PP1*S1*CF1
      P1(2) = PP1*S1*SF1
      P1(3) = PP1*C1
      P1(4) = EE1
C
      C12 =(PP3*PP3 - PP2*PP2 - PP1*PP1)/(2.*PP1*PP2)
      S12 = DSQRT(1.- C12*C12 )
      F12 = PI2*RNDM(-1.)
      CF12= DCOS(F12)
      SF12= DSIN(F12)
      X2  = PP2*S12*CF12
      Y2  = PP2*S12*SF12
      Z2  = PP2*C12
C
      P2(1) = C1*CF1*X2 - SF1*Y2 + S1*CF1*Z2
      P2(2) = C1*SF1*X2 + CF1*Y2 + S1*SF1*Z2
      P2(3) =-S1*X2              + C1*Z2
      P2(4) = EE2
C
      CALL ARTURS(P1(1),PL0(1),PL1(1))
      CALL ARTURS(P2(1),PL0(1),PL2(1))
C
      DO 30 I=1,4
   30 PL3(I) =PL0(I) - PL1(I) - PL2(I)
C
      RETURN
      END
C
      SUBROUTINE DECAYS(P0,P1,P2)
C...........................................................................
C   TO GENERATE ISOTROPIC (IN C.M.S.) DECAY OF ONE PARTICLE INTO TWO ONES
C   INPUT  VALUES:   P0(1-4) AND P0(5),P1(5),P2(5) - MASSES OF PARTICLES
C   OUTPUT VALUES:   P1(1-4),P2(1-4)
C   THE SUBROUTINE CALLS SUBR. ARTURS TO DO LORENZ TRANSFORMATION
C
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION P0(5),P1(5),P2(5),PP1(5)
      REAL RNDM 
      DATA PI2/6.28318530718/
C
      E1=(P0(5)**2 + P1(5)**2 - P2(5)**2 )/(2*P0(5))
      E2= P0(5) - E1
      P = DSQRT(  E1**2 - P1(5)**2 )
      CT= 2.*RNDM(-1.) -1.
      ST= DSQRT(1.-CT**2)
      FI= RNDM(-1.)*PI2
      PP1(1)=P*ST*DCOS(FI)
      PP1(2)=P*ST*DSIN(FI)
      PP1(3)=P*CT
      PP1(4)=E1
      CALL ARTURS(PP1(1),P0(1),P1(1))
      DO 1 I=1,4
    1 P2(I) = P0(I) - P1(I)
      RETURN
      END
C
      SUBROUTINE ARTURS(PC,P0,PL)
C...........................................................................
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION PC(5),P0(5),PL(5)
C
      AM0 =  P0(5)
      EL4 = (PC(4)*P0(4) + PC(3)*P0(3) + PC(2)*P0(2) + PC(1)*P0(1))/AM0
      DO 10 I = 1,3
   10 PL(I) = PC(I) + P0(I)*(EL4+PC(4))/(P0(4)+AM0)
      PL(4) = EL4
      RETURN
      END
C
      SUBROUTINE ABTOCDS(PA,MB,PC,PD,T)
C............................................................................
C      TO CALCULATE THE REACTION A+B INTO C+D ( PA - ALONG Z AXIS )
C      WHERE THE MOMENTUM OF B IS EQUAL ZERO.
C      INPUT VALUES : THE MASSES OF ALL THE PARTICLES IN PI(5) , PA(1-4) AND T
C      OUTPUT VALUES:  PC(1-4) AND PD(1-4)
C
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION PA(5),PC(5),PD(5)
      DOUBLE PRECISION MB
      REAL RNDM
      DATA PI2 / 6.28318530718 /
C
      PASQ = PA(1)**2+PA(2)**2+PA(3)**2
      PPA  = DSQRT(PASQ)
      SQMA = PA(5)**2
      SQMB =    MB**2
      SQMC = PC(5)**2
      SQMD = PD(5)**2
C
      EA    = DSQRT(SQMA+PASQ)
      ED    = (SQMB+SQMD-T)/(2.*MB)
      EC    = EA+MB-(SQMB+SQMD-T)/(2.*MB)
C      
      PPC   = DSQRT(EC**2-SQMC)
      DCOSAC =(EA*EC -.5*(SQMA+SQMC-T))/(PPA*PPC)
      DCOSQ  = DCOSAC**2
      IF(DCOSQ.GT.1.) THEN
                      T=100.
	       RETURN
      ENDIF
C 
      PCTR  = DSQRT(1.-DCOSQ)*PPC
      FI    = PI2*RNDM(-1.)
      PC(1) = PCTR*DSIN(FI)
      PC(2) = PCTR*DCOS(FI)
      PC(3) = PPC*DCOSAC
      PC(4) = EC
      DO 1 I=1,4
    1 PD(I) = PA(I)-PC(I)
      PD(4) = PD(4)+MB
      RETURN
      END
C
      subroutine reaction(Nreson, T, MissMass)
C...............................................................................
C      for given resonance ruturns trans. momentum and missing mass
C      distributions calculated by M. Stolpovsky
C      f2 calculated 30.06.11 by SA and me
      integer Nreson
      double precision R1, R2, T, b, Random
      double precision MissMass, Mean, s2, Double_MM_distr_2pi0,FF
      double precision Double_T_distr_f2,Double_MM_distr_f2
C
      if (Nreson.eq.1) then            !  resonance = pi0
C
      b = 2.6129
  134 Random = RNDM(-1.) 
      T = (1./b)*dlog(Random)         !  rho = (1/b)*exp(-b*x)
      if(T.gt.(-0.1)) go to 134
  111 if (RNDM(-1.).lt.(0.493544008)) then 
         Mean = 2.9667
         s2 = 0.40686
      else
         Mean = 2.1106
         s2 = 0.79967
      endif
      MissMass = Mean+s2*rnd_gauss(1.)  !  rho = (1/(sqrt(2*pi)*s2))*exp(-(x-mean)**2/(2*s2**2))
           
      if (MissMass.gt.(3.5)) go to 111
      if (MissMass.lt.(0)) go to 111
     
      endif
C
      if (Nreson.eq.2) then            !  resonance = eta
C      
      b = 2.6305
  234 Random = RNDM(-1.)
      T = (1./b)*dlog(Random)
      if(T.gt.(-0.1)) go to 234
      Mean = 1.477
      s2 = 0.67981
  222 MissMass = Mean+s2*rnd_gauss(1.)
      if (MissMass.gt.(3.5)) go to 222
      if (MissMass.lt.(0)) go to 222
      !resonance = eta
      endif 
C
      if (Nreson.eq.4) then            !  resonance = K0s
C
      b = 2.9392
      Random = RNDM(-1.)
      T = (1./b)*dlog(Random)
      Mean = 1.7538
      s2 = 0.63152
  333 MissMass = Mean+s2*rnd_gauss(1.)
      if (MissMass.lt.(0)) go to 333
      endif 
C
      if (Nreson.eq.5) then            !  resonance = f2
C
 434  R1 = RNDM(-1.)
      R2 = RNDM(-1.)
      T = R1*2.
      if((R2*500.).gt.Double_T_distr_f2(T)) go to 434
      T = -T
 444  R1 = RNDM(-1.)
      R2 = RNDM(-1.)
      MissMass = R1*2.7
      if((R2*35.).gt.Double_MM_distr_f2(MissMass*1000.))go to 444
      endif 
C
      if (Nreson.eq.6) then            !  resonance = 2pi0
C
      b = 3.26
  534 Random = RNDM(-1.)
      T = (1./b)*dlog(Random)
      if(T.gt.(-0.15)) go to 534
  555 R1 = RNDM(-1.)
      R2 = RNDM(-1.)
      FF=Double_MM_distr_2pi0(R1*3400)
C      write(*,*) 'MM distr = ', FF
      if ((R2*800.).gt.FF) go to 555
      MissMass = R1*3.4
      if (MissMass.gt.(3.5)) go to 555
      if (MissMass.lt.(0)) go to 555
      endif 
C
      if (Nreson.eq.7) then            !  resonance = f0(500) -> 2pi0
C      
      write(*,*) 'Hyp_Carlo: Resonance nb.', Nreson, ' is not in stream'
      stop 'Hyp_Carlo: error in channel number f0(980)'
      endif
C      
      return
      end
C
C     
      function rnd_gauss(sigma)
c random gauss distribution with dispersion SIGMA
      save pi,pi2,emi
      data pi/3.14159265358979/,pi2/-1./,emi/-1./
      
      if(pi2.lt.0.)then
       pi=acos(-1.)
	pi2=pi*2.
c         if(emi.lt.0.)emi=alog(0.33e-30)       ! ICL 1906A (word - 48 bits)
	 if(emi.lt.0.)emi=2.73501842E-20  ! PC        (word - 32 bits)
	endif
	
    2    R=RNDM(-1.)
c        R=R+(1.-R)*emi
c        if(R.lt.0.33E-30)R=0.33E-30
        if(R.lt.emi)goto 2
        R=-2.*alog(R)
	if(R.lt.0.)R=0.
	R=sqrt(R)*sigma        
        PHI=RNDM(-1.)*pi2
 	rnd_gauss=COS(PHI)*R ! NORMAL DISTRIB.
c	 y=SIN(PHI)*R
      RETURN
      END
C
      function Double_MM_distr_2pi0(xx) ! Double mean returning variable type, just remind for me
C  2pi0 missing mass distribution by M. Stolpovsky
C  Max~750
      
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)    
C      double precision MM_distr_2pi0
      if(xx.lt.0) go to 742
      if(xx.gt.3400.) go to 742
      par1 = 552.90
      par2 = 34.823
      par3 = 8.5308
      par4 = 673.05
      par5 = 24.683
      par6 = 7.8292
      Func1 = par1*DEXP(-0.5*((dsqrt(xx)-par2)/par3)**2)
      Func2 = par4*DEXP(-0.5*((dsqrt(3400.-xx)-par5)/par6)**2)
      Double_MM_distr_2pi0 = Func1 + Func2
C      write(*,*) 'MM distr in function = ', Double_MM_distr_2pi0
      return
C
  742 continue
      Double_MM_distr_2pi0 = 0.
      return 
      end
C
      function Double_Mass_distr_2pi0(xx)
C  2pi0 effective mass distribution by M. Stolpovsky
C  Max~750     
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)    
      if(xx.lt.270) go to 743
      if(xx.gt.1600) go to 743
      par1 = 17.402
      par2 = -.0078655
      par3 = -.0044932
      par4 = .0050605
      par5 = .0044287
      Func1 = par1*DEXP(par2*xx)*(par3+par4*xx+par5*xx*xx)
      Double_Mass_distr_2pi0 = Func1 
C      write(*,*) 'Mass distr in function = ', Double_Mass_distr_2pi0
      return
C
 743  continue
      Double_Mass_distr_2pi0 = 0.
      return 
      end
C
      function Double_T_distr_f2(xx) ! here xx in GeV, not MeV!!!!!
C     f2 T-distr. calculated 30.06.11 by SA and me
C     max~500
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)    
      if(xx.lt.0.) go to 744
      if(xx.gt.2.) go to 744
      par1 = 38.304
      par2 =-5.8335
      par3 = 2.5510
      par4 = 180.27
      par5 =-309.88
      par6 = 630.54
      Func1=par1*DEXP(par2*xx)*(par3+par4*xx+par5*xx*xx+par6*(xx**3))
      Double_T_distr_f2=Func1
      return
C
 744  continue
      Double_T_distr_f2=0.
      return
      end
C
      function Double_MM_distr_f2(xx)
C     f2 T-distr. calculated 30.06.11 by SA and me
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)    
      if(xx.lt.0.) go to 745
      if(xx.gt.2700.) go to 745
      par1 = 0.067956
      par2 = 0.019954
      par3 = 0.40668E-04
      par4 = -0.37954E-07
      par5 = 0.74093E-11
      Double_MM_distr_f2=par1+par2*xx+par3*xx*xx+par4*(xx**3)+
     &     par5*(xx**4)
      return
C
 745  continue
      Double_MM_distr_f2=0.
      return
      end
