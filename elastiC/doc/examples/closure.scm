(define (create a)
  (let ((b 10))
    (lambda (c)
      (* a (* b c)))))


(display (let ((func1 (create 2))
	       (func2 (create 5)))
	   (list (func1 3) (func2 3))))
(display "\n")

