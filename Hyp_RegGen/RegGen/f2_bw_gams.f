      function f2_bw_gams(X)
C     BW from the GAMS paper: Eur.Phys.J. A3, 361-371 (1998)    
C                                        S.Sadovsky 17.08.2011
      common/PAWPAR/ Par(10)
c
      R0 = 0.197326               !  1/GeV
      Ampi0  = 0.1349766
c
      ppi0   = sqrt(abs((Par(2)/2.)**2-Ampi0**2))
      ppiX   = sqrt(abs((X/2.)**2     -Ampi0**2))
c    
      D2pi0  = 9.+3.*(ppi0/r0)**2+(ppi0/r0)**4
      D2piX  = 9.+3.*(ppiX/r0)**2+(ppiX/r0)**4
c
      Gamma  = Par(3)*(ppiX/ppi0)**5*D2pi0/D2piX                  
      f2_bw  = Par(2)**2*Gamma**2/((x*x-Par(2)**2)**2+(Par(2)*Gamma)**2)
      f2_bw_gams=Par(1)*f2_bw*X**2/ppiX
c
      return
      end
