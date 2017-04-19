C dyn_export_test.f77
C this file is part of Dynstack/RemoteControl for CORSIKA
C
C Copyright (C) <2016> <Dominik Baack>
C All rights reserved.
C
C     This software may be modified and distributed under the terms
C     of the LGPL license. See the LICENSE file for details.
C


      INTEGER FUNCTION test_dyn_export()

      REAL particle*13
      CHARACTER LINE*512
      INTEGER RET


      CALL dynstack_init("DYNSTACK 10000", &size);

      CALL dynstack_start();

      DO 100 I=1, 25 
         DO 101 J=1, 13
            particle(J) = I*J
  101    CONTINUE
         CALL dynstack_tstout(particle, LEN(particle), RET)
  100 CONTINUE

      DO 102 I=1, 25
         CALL dynstack_fstack(particle, LEN(particle) )        
         DO 103 J=1, 13
            IF (particle(J) .NE. I*J) 
               WRITE(*,*) 'Not equal',J,' ',I*J
    
            END IF
  103    CONTINUE       
  102 CONTINUE

      CALL dynstack_end();

      return 0
      END
