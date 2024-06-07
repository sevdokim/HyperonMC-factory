      function eg_in_pi0(X)
C      
C     Sdv 14.11.2019: this is probability parametrization of gamma energy 
C                     in 2g decay of pi0 at energy Epi0 > 1500 MeV
C ........................................................................       
      common/pawpar/par(10)
      data par / 58958., -0.00088060, 6.5486, 13570., 4244.4,
     +          -6624.5,    24773.,  -4380.7, 7631.9,    0.0/
C
C-       par(1) = 70000.
C-       par(2) =-0.000697
C-       par(3) =  300.
C-       par(4) = 6000.
C-       par(5) =  400.
C
      if(X.lt.1275.) then
                     eg_in_pi0 = 21663. - 1.2405*X
           else
	   eg_in_pi0=par(1)*exp(par(2)*X)
     +             +(par(3)*exp(-(X-par(4))**2/(2.*par(5)**2))
     +             + par(6)*exp(-(X-par(7))**2/(2.*par(8)**2)))*
     +           abs(par(9)-X)
       endif
       eg_in_pi0=eg_in_pi0/3800.
       return
       end
