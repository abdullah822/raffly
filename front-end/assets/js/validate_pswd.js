
var password = document.getElementById("psw-formbuilder-s"),
  confirm_password = document.getElementById("psw2-formbuilder-s");

function validatePassword() {
  if (password.value != confirm_password.value) {
    confirm_password.setCustomValidity("Passwords Don't Match");
  } else {
    confirm_password.setCustomValidity('');
  }
}

password.onchange = validatePassword;
confirm_password.onkeyup = validatePassword;
