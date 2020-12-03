
function showProduct(tok){

   fetch('../middle/php/getRafflesComplete.php?t='+tok, {
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
        const { id, title, description, startTime, endTime, company, productImage, quantity, size, format, unit_cost, productName, } = raffle;
        end = new Date(endTime.$timestamp.t * 1000);
        var cost = unit_cost.toString().replace('$', "");
        raffle_id = id.$oid;
        count++;
        var ids=tok.slice(6);
        if(ids == raffle_id){ 
        result +=
          `<div class="card col-6 p-3 " style="display:block; color:white; margin: 0 auto; ">
              <div id="prod-${count}"></div>
                <span><h5 style="color:white;"> ${title} </h5></span>
                  <div class="" >
                    <div class="card-wrapper">
                      <div class="card-img">
                        <img src="${productImage}" alt="" title="">
                      </div>
                      <div class="card-box" style="padding:4px;">
                        <p class="mbr-text mbr-fonts-style display-7" style="padding:4px;">${description}</p>
                        <div style="text-align:center;">
                          <form name="${raffle_id}" id="${raffle_id}" action="https://www.paypal.com/cgi-bin/webscr" method="post" target="_top">
                          <input type="hidden" name="cmd" value="_xclick">
                          <input type="hidden" name="business" value="rafflyraffles@gmail.com">
                          <input type="hidden" name="lc" value="US">
                          <input type="hidden" name="item_name" value="${productName}">
                          <input type="hidden" name="item_number" value="${raffle_id}">
                          <input type="hidden" name="amount" value="${cost}">
                          <input type="hidden" name="currency_code" value="USD">
                          <input type="hidden" name="button_subtype" value="services">
                          <input type="hidden" name="no_note" value="0">
                          <input type="hidden" name="tax_rate" value="0.070">
                          <input type="hidden" name="shipping" value="10.00">
                          <input type="hidden" name="bn" value="PP-BuyNowBF:btn_paynowCC_LG.gif:NonHostedGuest">`;
        if (size !== false) {
          result += `<table style="text-align:center; width:100%;">
            <tr><td>
            <input type="hidden" name="on0" value="Sizes"></td></tr><tr><td>Sizes:&nbsp<select name="os0" style="border-radius:8px; padding:4px;" required="required"> <option value="">Select</option>`;
          for (var k = 0; k < size.length; k++) { 
          result += `<option value="${size[k]}">${size[k]}</option>`; }
        }
        result += `</select></td></tr>
        </table>
        <button class="btn btn-primary btn-small" style="padding:0.5rem 2rem;">Buy</button>&nbsp&nbsp&nbsp
                      </form>   
                      </div>
                  </div>
              </div>
          </div>
        </div>`;}
        
        document.getElementById('result').innerHTML = result;
      });
    })
    .catch(err => console.log('Request Failed', err)); // Catch errors
    document.getElementById("token").value = "";
}
