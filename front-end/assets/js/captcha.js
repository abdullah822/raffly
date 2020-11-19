function onClick() {
  // e.preventDefault();
  grecaptcha.ready(function () {
    grecaptcha.execute('6Ld3jNkZAAAAAH2ThcRFyDXkiNCyr1GjJrYqeJOY', { action: 'submit' }).then(function (token) {
      console.log(token);
      document.getElementById("g-recaptcha-response-formbuilder-s").value = token;
      //document.getElementById("registration").submit();
    });
  });
}
