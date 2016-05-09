
  // Global constants
  var group = false;
  var choosen_metric = 'euclidean';
  var choosen_smooth = 'landmarks';
  var outlier_count=1;
  var method="2phase"

  // imports and setups
  _.mixin(_.str.exports());
  $.ajaxSetup({
    timeout: 1000000,
  });


  var sd_header, sd_data;


  function getData(query) {
	  console.log("FE get data");
    console.log(query.length);
    $('div#container').empty();
    $('#compare-menu').html('<li><a href="#" class="active" data-id="-1">Show All</a></li>');
 //   console.log(JSON.stringify(query));
    $.get('/getgroups', JSON.stringify(query), processesBackEndData, 'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /getgroups');
      });
  }
  
 
  function formQuery(databasename){
	  this.databasename=databasename;
  }
  
  function getInterfaceFormData(query){
	  console.log("xdata"+query);
	  q = new formQuery(query);
	  $.get('/getformdata',JSON.stringify(q),processFormData,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /Formdata');
      });
  }

  function get_sim(r,c,n) {
    if (r > c) {
      var tmp = r;
      r = c;
      c = tmp;
    }
    return r*((n-1-(r-1)) + (n-1))/2 + (c-r-1);
  }

  function getDataNow() {
    getData("select uniqueCarrier,DayofMonth,avg(ArrDelay) from db.ontime group by uniqueCarrier,DayofMonth",outlier_count,method);
  }