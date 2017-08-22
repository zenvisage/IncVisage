var __slice = Array.prototype.slice;

function Point(x, y){
    this.x=x;
    this.y=y;
}

function SketchPoints(){
	this.points=[];
	this.minX=5;
	this.maxX=385;
	this.minY=10;
	this.maxY=190;
}


var listOfPoints = [];
var sketchPoints = new SketchPoints();

(function($) {
  var Sketch;
  $.fn.sketch = function() {
      //console.log("callfunction");
    var args, key, sketch;
    key = arguments[0], args = 2 <= arguments.length ? __slice.call(arguments, 1) : [];
    if (this.length > 1) {
      $.error('Sketch.js can only be called on one element at a time.');
    }
    sketch = this.data('sketch');
    if (typeof key === 'string' && sketch) {
      if (sketch[key]) {
        if (typeof sketch[key] === 'function') {
          return sketch[key].apply(sketch, args);
        } else if (args.length === 0) {
          return sketch[key];
        } else if (args.length === 1) {
          return sketch[key] = args[0];
        }
      } else {
        return $.error('Sketch.js did not recognize the given command.');
      }
    } else if (sketch) {
      return sketch;
    } else {
      this.data('sketch', new Sketch(this.get(0), key));
      return this;
    }
  };
  Sketch = (function() {
     // console.log("sketchfunction");
    function Sketch(el, opts) {
      this.el = el;
      this.canvas = $(el);
      this.context = el.getContext('2d');
      this.options = $.extend({
        toolLinks: true,
        defaultTool: 'marker',
        defaultColor: '#000000',
        defaultSize: 5
      }, opts);
      this.painting = false;
      this.color = this.options.defaultColor;
      this.size = this.options.defaultSize;
      this.tool = this.options.defaultTool;
      this.actions = [];
      this.action = [];
      this.canvas.bind('click mousedown mouseup mousemove mouseleave mouseout touchstart touchmove touchend touchcancel', this.onEvent);
      if (this.options.toolLinks) {
        $('body').delegate("a[href=\"#" + (this.canvas.attr('id')) + "\"]", 'click', function(e) {
          var $canvas, $this, key, sketch, _i, _len, _ref;
          $this = $(this);
          $canvas = $($this.attr('href'));
          sketch = $canvas.data('sketch');
          _ref = ['color', 'size', 'tool'];
          for (_i = 0, _len = _ref.length; _i < _len; _i++) {
            key = _ref[_i];
            if ($this.attr("data-" + key)) {
              sketch.set(key, $(this).attr("data-" + key));
            }
          }
          if ($(this).attr('data-download')) {
            sketch.download($(this).attr('data-download'));
          }
          return false;
        });
      }
    }
    Sketch.prototype.download = function(format) {
      var mime;
      format || (format = "png");
      if (format === "jpg") {
        format = "jpeg";
      }
      mime = "image/" + format;
      return window.open(this.el.toDataURL(mime));
    };
    Sketch.prototype.set = function(key, value) {
      this[key] = value;
      return this.canvas.trigger("sketch.change" + key, value);
    };
    Sketch.prototype.startPainting = function() {
      this.painting = true;
      sketchPoints = new SketchPoints();
      return this.action = {
        tool: this.tool,
        color: this.color,
        size: parseFloat(this.size),
        events: []
      };
    };
    Sketch.prototype.stopPainting = function() {
       // console.log("stopPainting");
      if (this.action) {
        this.actions.push(this.action);
      }
      this.painting = false;
      this.action = null;
      return this.redraw();
    };
    Sketch.prototype.onEvent = function(e) {
      //console.log("callonEvent");
      if (e.originalEvent && e.originalEvent.targetTouches) {
        e.pageX = e.originalEvent.targetTouches[0].pageX;
        e.pageY = e.originalEvent.targetTouches[0].pageY;
      }
      //if($.sketch.tools[$(this).data('sketch').tool]!=$.sketch.tools.submit){
      //console.log("test");
      $.sketch.tools[$(this).data('sketch').tool].onEvent.call($(this).data('sketch'), e);
      e.preventDefault();
      return false;
     // }
    };
    Sketch.prototype.redraw = function() {
     // console.log("callredraw");
      var sketch;
      this.el.width = this.canvas.width();
      this.context = this.el.getContext('2d');

     // console.log(this);
      sketch = this;

      $.each(this.actions, function() {
        if (this.tool) {
          return $.sketch.tools[this.tool].draw.call(sketch, this);
        }
      });
      if (this.painting && this.action) {
        return $.sketch.tools[this.action.tool].draw.call(sketch, this.action);
      }
    };
    return Sketch;
  })();



  $.sketch = {
    tools: {}
  };

  


  $.sketch.tools.marker = {
    onEvent: function(e) {
      switch (e.type) {
        case 'mousedown':
        case 'touchstart':
          this.startPainting();
          break;
        case 'mouseup':
        case 'mouseout':
        case 'mouseleave':
        case 'touchend':
        case 'touchcancel':
          this.stopPainting();
      }
      if (this.painting) {
        this.action.events.push({
          x: e.pageX - this.canvas.offset().left,
          y: e.pageY - this.canvas.offset().top,
          event: e.type
        });
        return this.redraw();
      }
    },
    draw: function(action) {
      var event, previous, _i, _len, _ref;
      this.context.lineJoin = "round";
      this.context.lineCap = "round";
      this.context.beginPath();
      this.context.moveTo(action.events[0].x, action.events[0].y);
      _ref = action.events;
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        event = _ref[_i];
        this.context.lineTo(event.x, event.y);
        //var thePoint = 
        sketchPoints.points.push(new Point(event.x, sketchPoints.maxY-event.y));
        previous = event;
      }
      this.context.strokeStyle = action.color;
      this.context.lineWidth = action.size;
      return this.context.stroke();
    }
  };
  return $.sketch.tools.eraser = {
    onEvent: function(e) {
      return $.sketch.tools.marker.onEvent.call(this, e);
    },
    draw: function(action) {
        //console.log("calldrawfunctioneraser");
      listOfPoints = [];
      sketchPoints = new SketchPoints();
      var oldcomposite;
      oldcomposite = this.context.globalCompositeOperation;
      this.context.globalCompositeOperation = "copy";
      action.color = "rgba(0,0,0,0)";
      $.sketch.tools.marker.draw.call(this, action);
      return this.context.globalCompositeOperation = oldcomposite;
    }

  };


//$(document).ready(function() {
   

//});

})(jQuery);



function trendAnalysis(){
	 console.log("hello");
     //this.stopPainting();
     var Data = [];
     var col1 = 0; var c1 = 0;
     var col2 = 0; var c2 = 0;
     var col3 = 0; var c3 = 0;
     var col4 = 0; var c4 = 0;
     //var tempxmin=0;var tempymin=0; var tempxmax=0;var tempymax=0;
     // var a = Math.round((listOfPoints[2].x)/136.3+1);
     //var b = Math.round((4-(listOfPoints[2].y)/102.25)*100)/100;
     //console.log(listOfPoints[2].x);
     //console.log(listOfPoints[2].y);
     for(var i=0; i<listOfPoints.length; i++){
       //if(listOfPoints[i].x<tempxmin) tempxmin= listOfPoints[i].x; if(listOfPoints[i].y<tempymin) tempymin= listOfPoints[i].y;if(listOfPoints[i].x>tempxmax) tempxmax= listOfPoints[i].x;if(listOfPoints[i].y>tempymax) tempymax= listOfPoints[i].y;
    
         var X = Math.round((listOfPoints[i].x)/102.25);
         var Y = Math.round((4-(listOfPoints[i].y-4)/51)*100)/100;
         //console.log(Y);
       if(X == 1){ col1= col1+Y; c1++;}
       if(X == 2){ col2 = col2+Y; c2++;}
       if(X == 3) {col3 = col3+Y; c3++;}
       if(X == 4) {col4 = col4+Y; c4++;}
     
     }
    // console.log(c2,Math.round((col2/c2)*100)/100);
     //console.log(tempxmin,tempymin,tempxmax,tempxmax);
     if(c1!=0) Data.push(new Point(1, Math.round((col1/c1)*100)/100));
     else Data.push(new Point(-1,-1));
     if(c2!=0) Data.push(new Point(2, Math.round((col2/c2)*100)/100));
     else Data.push(new Point(-1,-1));
     if(c3!=0) Data.push(new Point(3, Math.round((col3/c3)*100)/100));
     else Data.push(new Point(3,-1));
     if(c4!=0) Data.push(new Point(4, Math.round((col4/c4)*100)/100));
     else Data.push(new Point(4,-1));
     //console.log(Data[0], Data[1], Data[2], Data[3]);
      //console.log(Data);
      //console.log(Data[1]);

      var jsonData = [];
      var DataX =[];
      var DataY =[];
      //tempx = 0;  tempy = 0;
      var imported = document.createElement('script');
      imported.src = 'comboboxes.js';
      document.head.appendChild(imported);
      var myQuery = getSimilarTrendData();
      myQuery.sketchPoints=sketchPoints;


/*      
     for(var i=0; i<Data.length; i++){
       myQuery.dataX.push(Data[i].x);
       myQuery.dataY.push(Data[i].y);
      // console.log(jsonData[i]);
         //if(Data[i]>tempx)
         //  tempx = Data[i];
       // if(Data[i+1]>tempy)
         // tempy = Data[i+1];
     //return jsonData;




     }
*/     
      
      getData(myQuery);
    
	
	
}



