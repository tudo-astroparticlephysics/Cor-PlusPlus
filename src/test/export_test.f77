      INCLUDE 'export/rc_export_test.f77'

      INCLUDE 'export/dyn_export_test.f77'

      PROGRAM TIMETEST
      INTEGER RET

      WRITE(*,1)
    1 FORMAT("--EXPORT TEST--")

      WRITE(*,2)
    2 FORMAT('REMOTE CONTROL')
      RET = test_rc_functions()
      
