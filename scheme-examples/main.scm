(load "import.scm")
(import "root.scm")
(import "html.scm")
(import "models.scm")

(define my-script
  (script ""))

(define basic-template
  (lambda (content)
    (string-append 
      doctype
      (html 
        (head
          (title "Dan's Website")
          (link 
            (list 
              '("rel" "stylesheet")
              `("href" ,(string-append root "/style.css")))))
        (body 
          (div 
            (list '("id" "main-content"))
              navbar
          content
          my-script))))))
          ;; (p 
          ;;   (list 
          ;;     '("width" "500") 
          ;;     '("style" "color:red; border: 1px solid black"))
          ;;   "some stuff"))))))

;; (define output
;;   (html-template 
;;     basic-template
;;     (a (list '("href" "/Users/dtuveson/Programming-Projects/thing/scheme-examples/about.html")) "hi") 
;;     (p "hello")
;;     (do 
;;       ((i 1 (+ 1 i))
;;        (content "")) 
;;       ((= i 10) content)
;;       (set!
;;         content
;;         (string-append
;;           content 
;;           (br)
;;           (div 
;;             "hello for the " (number->string i) "th time"))))
;;     (div 
;;       (p "more stuff")
;;       (br))
;;     (p "and another!")))

(display (basic-template (div "")))
