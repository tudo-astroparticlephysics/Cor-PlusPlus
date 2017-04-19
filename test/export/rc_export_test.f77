C   rc_export_test.f77
C   this file is part of Dynstack/RemoteControl for CORSIKA
C  
C   Copyright (C) <2016> <Dominik Baack>
C   All rights reserved.
C 
C   This software may be modified and distributed under the terms
C   of the LGPL license. See the LICENSE file for details.
C 

      INTEGER FUNCTION test_rc_export()

      CHARACTER LINE*512
      REAL EVTH*273
      INTEGER LINE_LENGTH


      LINE = ' '
C     LINE = 'REMOTECONTROL_IP 127.0.0.1 10004'
      READ(5,500) LINE
 500  FORMAT(A)
      CALL remotecontrol_init(LINE, len(LINE))


      LINE = ' '
C     LINE = 'REMOTECONTROL_P asd'
      READ(5,501) LINE
 501  FORMAT(A)
      CALL remotecontrol_init(LINE, len(LINE) );

      CALL remotecontrol_start_();

      CALL remotecontrol_send(1, 'test\0', 5);
      CALL remotecontrol_send(7, 'TEST2\0', 6);

      DO 20, J = 1,273 
         EVTH(J) = J
   20 CONTINUE

      CALL remotecontrol_push_evth(EVTH);

      CALL remotecontrol_end();


      CALL remotecontrol_steering_nextline(LINE, LEN(LINE), LINE_LENGHT);

      RETURN 0

C           remotecontrol_push_runh_(const float* data);
C           remotecontrol_push_rune_(const float* data);
C           remotecontrol_push_evth_(const float* data);
C           remotecontrol_push_evte_(const float* data);
C           remotecontrol_push_initalparticle(const float* data, unsigned int len);
C           remotecontrol_recv_initalparticle(const float* data);
      END
