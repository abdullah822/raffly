fetch('../middle/php/getRaffles.php', {
  method: "GET",
  headers: {
    'Content-Type': 'application/json',
    'Accept': 'application/json'
  }
})
  .then((res) => { return res.json() })
  .then((data) => {
    count = 0;
    let result = `<div></div>`;
    finish = [];
    data.forEach((raffle) => {
      const { id, title, description, startTime, endTime, company, productImage, quantity, size, format, unit_cost, productName, } = raffle
      start = new Date(startTime.$timestamp.t * 1000);
      end = new Date(endTime.$timestamp.t * 1000);
      expire = '';
      raffle_id = id.$oid;
      count++;
      result +=
        `<div class="card col-12 col-md-6 p-3 col-lg-4" style="display:inline-block; color:white;">
                      <div id="prod-${count}"></div>
                    <span><h5 style="color:white;"> ${title} </h5></span>
                    
                      <div class="" >
                                <div class="card-wrapper">
                                    <div class="card-img">
                                        <img src="${productImage}" alt="" title="">
                                    Expires:&nbsp<span class="" name="countdown[]" id="countdown-${count}" ></span>
                                    </div>
                                    <div class="card-box" style="padding:4px;">
                                        <p class="mbr-text mbr-fonts-style display-7" style="padding:4px;">${description}</p>
                                        <div style="text-align:center;">
                                        <form name="form-${count}" id="form-${count}" style="display:none;">
                                        <input type="hidden" name="raffle_id" value="${raffle_id}">`;
      if (size !== false) {
        result +=
          `<label for="size">Size:</label>
                          <select name="size" id="size-${count}">`;

        for (var k = 0; k < size.length; k++) { result += `<option value="${size[k]}">${size[k]}</option>`; }
      }
      result += `</select>
                            <button onclick=" enterRaffle(event,${count});" class="btn btn-white-outline" style="padding:0.5rem 2rem;">Enter</button>&nbsp&nbsp&nbsp
                                        </form> 
                                        <button type="button" class="btn btn-white-outline" onclick="getPopup(${count})">${unit_cost}</button>  
                                        </div>
                                    </div>
                                </div>
                            </div>
                  </div>`;
      finish.push(end);
      document.getElementById('result').innerHTML = result;
    });
    setInterval(function () {
      var now = new Date();
      var left = new Date(end - now);
      var months = left.getMonth() + 1;
      var days = left.getDate().toString();
      var hours = left.getHours().toString();
      var minutes = left.getMinutes().toString();
      var seconds = left.getSeconds().toString();
      expire = months + "M " + days + "d " + hours + "h " + minutes + "m " + seconds + "s";
      for (var i = 0; i < finish.length; i++) {
        document.getElementById("countdown-" + (i + 1)).innerHTML = expire;
      }

    }, 1000);
  })
  .catch(err => console.log('Request Failed', err)); // Catch errors