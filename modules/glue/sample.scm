;; time.h interface

;; module - introduces a new module
;;  first parameter is module name
;;  second parameter (optional) is prefix for C symbols
(module "time" "ec_time_")

(c-include "time.h")

(c-atomic-type "clock_t")
(c-atomic-type "time_t")

;; c-literal introduces literal text in the generated module
(c-literal "....")

;; include includes a file directly in the generated module
;; (while c-include produces an #include directive)
(include "....")

;; import processes another idl file
(import "...")

; structs

(c-struct timespec
    (fields
     (long tv_sec)
     (long tv_nsec)))

(c-struct tm
    (fields
     (int tm_sec)
     (int tm_min)
     (int tm_hour)
     (int tm_mday)
     (int tm_mon)
     (int tm_year)
     (int tm_wday)
     (int tm_yday)
     (int tm_isdst)
     ))

; functions

(c-function clock_t clock ())
(c-function time_t  time (((pointer-out time_t) timer)))
(c-function double  difftime ((time_t time1)
			      (time_t time0)))
(c-function time_t  mktime (((pointer-in tm) tp)))

(c-function size_t  strftime (((c-buffer 80) s)
			      (size_t maxsize)
			      (c-string format)
			      ((pointer-in tm) tp)))
; arrays ?
; (c-array 15 time_t)
