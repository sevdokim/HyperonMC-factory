      function omg_bw_gams(X)
C     BW from the GAMS paper: Eur.Phys.J. A3, 361-371 (1998)    
C                                        S.Sadovsky 17.08.2011
      common/PAWPAR/ Par(10)
C-    data Ampi0, R0  / 0,1349766,  0.12112 /	 ! pi0 mass and inverse R0 for omega in MeV
C-    data Rmas, Rwid / 0.78265,    0.00849 /	 ! omg parameters in GeV, not used here
C
      R0 = 0.12112               !  1/GeV
C-    R0 = 0.197326              !  1/GeV for f2
      Ampi0  = 0.1349766
C                                                                     
      ppi0   = sqrt(abs((Par(2)/2.)**2-Ampi0**2))
      ppiX   = sqrt(abs((X/2.)**2     -Ampi0**2))
C          
      D2pi0 = 1.+(ppi0/r0)**2                     ! Blatte-Weiskopf factor for omega
      D2piX = 1.+(ppiX/r0)**2
C
      Gamma  = Par(3)*(ppiX/ppi0)**3*D2pi0/D2piX                  
      Bw     = Par(2)**2*Gamma**2/((x*x-Par(2)**2)**2+(Par(2)*Gamma)**2)
      omg_bw_gams=Par(1)*Bw*X**2/ppiX
C
      return
      end


