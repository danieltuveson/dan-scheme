(import "strings.scm")

(define doctype "<!DOCTYPE html>")

(define html-attribute
  (lambda (input)
    (cond 
      ((string? input) 
       input)
      ((pair? input) 
       (let ((key (car input)) 
             (val (cadr input)))
         (string-append key "=\"" val "\""))))))

(define html-attributes
  (lambda (attributes)
    (concat-with-spaces 
      (map html-attribute attributes))))

(define html-element-single
  (lambda (name attributes)
    (string-append
      "<" name " "
      (html-attributes attributes)
      " />")))

(define html-element
  (lambda (args)
    (let ((len (length args)))
      (cond 
        ((eq? 0 len) 
         (display "Error: html-element takes at minimum one argument"))
        ((eq? 1 len)
         ; self-closing, no attributes
         (string-append "<" (car args) "/>"))
        ((and (eq? 2 len) (list? (cadr args)))
         ; self-closing, has attributes
         (string-append 
           "<" (car args) " " (html-attributes (cadr args)) "/>"))
        ((string? (cadr args))
         ; non-self-closing, no attributes
         (string-append
           (let 
             ((name (car args))
              (content (apply string-append (cdr args))))
             (string-append "<" name ">" content "</" name ">"))))
        (else
           (let 
             ((name (car args))
              (attributes (html-attributes (cadr args)))
              (content (apply string-append (cddr args))))
             (string-append "<" name " " attributes ">" content "</" name ">")))))))

(define html-template
  (lambda (template . contents)
    (template 
      (apply string-append contents))))

(define html
  (lambda content
    (html-element (cons "html" content))))

(define head
  (lambda content
    (html-element (cons "head" content))))

(define title
  (lambda content
    (html-element (cons "title" content))))

(define body
  (lambda content
    (html-element (cons "body" content))))

(define h1
  (lambda content
    (html-element (cons "h1" content))))

(define p
  (lambda content
    (html-element (cons "p" content))))

(define div
  (lambda content
    (html-element (cons "div" content))))

(define a
  (lambda content
    (html-element (cons "a" content))))

(define br
  (lambda content
    (html-element (cons "br" content))))

(define style
  (lambda content
    (html-element (cons "style" content))))

(define link
  (lambda content
    (html-element (cons "link" content))))

(define script
  (lambda content
    (html-element (cons "script" content))))
