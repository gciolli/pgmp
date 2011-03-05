(define (fact a n)
  (cond
   ((= n 1) a)
   ((> n 1) (fact (* a n) (- n 1)))))

(let ((my_number 32177))
  (time
   (display
    (string-length
     (number->string
      (fact 1 my_number))))
   (newline)))
