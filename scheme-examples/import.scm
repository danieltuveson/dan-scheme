;;; defines a function for safe importing by using a list to keep track of what has been imported. 

(define total-import-list (list "import.scm"))

(define import
  (lambda (filename)
    (if (not (member filename total-import-list))
       (begin
         (set! total-import-list (cons filename total-import-list))
         (load filename)
         total-import-list))))


