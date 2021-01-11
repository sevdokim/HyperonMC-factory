      PROGRAM MAIN
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C
      parameter        (npawc=250000)
      COMMON/PAWC/ HMEM(npawc)
      COMMON/LUN / Lunst
      data Lunout, Lunst / 8, 10 / 
C      
      real React, BeamMomentum
      common/beam_momentum/ BeamMomentum
      double precision Pout(10,500)
      common/rections/ Ireac1, Ireac2, Mcarlo, React(10,100) 
C-    data Ireac1, Ireac2 / 4, 55 /
C..................................      
C     
      CALL HLIMIT(npawc)
      CALL HROPEN(Lunst,'ions','reggen.hbook','N',1024,IOSTAT)
      CALL BOOKIN
C
      BeamMomentum = 7.2
      iseed     = 777555
C   
      do Iev=1,100000
         call RegGen_event(BeamMomentum,Ievent,Ierror,iseed,Jrec,IstOut,Pout) 
C
C-       write(*,*) 
C-       write(*,101) ((Pout(j,k),j=1,10),k=1,6)
C     
C-       call event_print(Lunout,Ievent,Ierror,Jrec,IstOut,Pout)
C
      if(1000*(iev/1000).eq.iev) write(*,*) 'MAIN: Iev =',Iev
C
      call analysis(Ievent,Ierror,Jrec,IstOut,Pout)     
      enddo
C      
      call hrout(0,icycle,' ')
      call hrend('ions')
      close (Lunst)
C
  101 format(5G20.13)
      stop'OK'
      END
C        
      subroutine analysis(Ievent,Ierror,Jrec,IstOut,Pout)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      double precision Pout(10,500)
      double precision Psm(5), Pbeam(5)
      real prtnb, react, Etot, Rmtot, RmRes, sin12,sngl
C
      react = Jrec
      prtnb = IstOut
      call hf1(10,react,1.)
C      
      if (Ierror.ne.0) return
      if (IstOut.le.1) return
C     
      call hf1(11,prtnb,1.) 
C
      do j=1,4
           Psm(j) = 0.
      enddo
C      
      do ip = 1,IstOut
      do j=1,4
      Psm(j) = Psm(j)+Pout(j,ip)
      enddo
      enddo
C      
c      write(*,*) 'Egamma=',Pout(4,2)+ Pout(4,3)  
      
      cos12 = Pout(1,2)*Pout(1,3)+Pout(2,2)*Pout(2,3)+Pout(3,2)*Pout(3,3)
      Ron1  = Pout(1,2)*Pout(1,2)+Pout(2,2)*Pout(2,2)+Pout(3,2)*Pout(3,2)
      Ron2  = Pout(1,3)*Pout(1,3)+Pout(2,3)*Pout(2,3)+Pout(3,3)*Pout(3,3)
      cos12 = cos12/sqrt(Ron1*Ron2)
C-    write(*,*) cos12,Ron1,Ron2,(1.-cos12)
      sin12 = dsqrt((1.-cos12)/2.)
      call hf1(25,sin12,1.0)
C      
      Etot = real(Psm(4))
      call hf1(20,Etot,1.)
      Amtot = Dsqrt(Psm(4)**2 - Psm(1)**2 - Psm(2)**2 - Psm(3)**2)
C      
      Rmtot = Amtot
      call hf1(21,Rmtot,1.) 
C
      do j=1,4
      Psm(j) = Psm(j)-Pout(j,1)
      enddo 
C          
      AmRes = Dsqrt(Psm(4)**2 - Psm(1)**2 - Psm(2)**2 - Psm(3)**2)
      RmRes = real(AmRes)
      call hf1(22,RmRes,1.)
      AmPi = 0.13956999778747559
      Pbeam(1)=0.
      Pbeam(2)=0.
      Pbeam(3)=7.2
      Pbeam(5)=AmPi
      Pbeam(4)=dsqrt(Pbeam(1)**2+Pbeam(2)**2+Pbeam(3)**2+Pbeam(5)**2)
      T = (Pbeam(4)-Psm(4))**2-(Pbeam(3)-Psm(3))**2-(Pbeam(2)-Psm(2))**2-(Pbeam(1)-Psm(1))**2
c      write(*,*)'T=',T
      call hf1(23,sngl(T),1.)
      AmisMass = dsqrt(Pout(4,1)**2-Pout(3,1)**2-Pout(2,1)**2-Pout(1,1)**2)
      call hf1(24,sngl(AmisMass),1.)
C      
      
      RETURN
      END
C
      SUBROUTINE BOOKIN      
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      parameter         (npawc=250000)
      COMMON/PAWC/  HMEM(npawc)
C 
      call hbook1( 10,'Reaction index           ',  100, 0.0, 100., 0.)
      call hbook1( 11,'Number of final particles',   50, 0.0,  50., 0.)
      call hbook1( 20,'Total energy             ',  500, 0.0, 10.0, 0.)
      call hbook1( 21,'Total mass               ', 4000, 0.0,  4.0, 0.)
      call hbook1( 22,'Total meson mass         ', 6000, 0.0,  3.0, 0.)
      call hbook1( 23,'Reaction T               ', 4000,-2.0,  2.0, 0.)
      call hbook1( 24,'Missing mass             ', 4000, 0.0,  4.0, 0.)
C
      call hbook1( 25,'sin12/2                  ', 5000, 0.0,  1.0, 0.)
C
C-    call hbook2(4225,'pi+/pi-: PhiTY vs pair Mass',250, 0.,2.5,
C-   +                                            100, 0.,6.283,0.)
C                  
      RETURN
      END
