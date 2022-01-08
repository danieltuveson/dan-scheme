(import "root.scm")
(import "html.scm")

(define headline-box
  (lambda ()
    (div "")))

(define navbar
  (div
    (list '("id" "navbar"))
    (a 
      (list 
        '("class" "navbar")
        `("href" ,(string-append root "")))
      "Hacker News")))

(define navbar-content
  (lambda (name link)
    (span
      (a '("href" link) name))))




