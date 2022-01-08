; concatenates a list of string with spaces in beteween
(define concat-with-seperator
  (lambda (seperator vals)
    (let loop ((str "")
               (values-remaining vals))
      (cond ((null? values-remaining) str)
            ((eq? str "")
             (loop (car values-remaining) (cdr values-remaining)))
            (else 
              (loop
                (string-append str seperator (car values-remaining))
                (cdr values-remaining)))))))
  
(define concat-with-spaces 
  (lambda (vals)
    (concat-with-seperator " " vals)))

