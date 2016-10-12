$('html').animate({"opacity":1}, 10);
$('html').css('background-color', '#61799a'); 

$( document ).ready(function() {
	updateHeight();

	window.onresize = function(event) {
		updateHeight();
	};

	$( "#logout" ).click(function() {
		$.ajax({
			type: "POST",
			url: "/logout"
		}).done(function(data) {
			$.ajax({
				type: "POST",
				url: "/"
			}).done(function(data) {
				$('html').animate({"opacity":0}, 500, function(){
					window.location.href = "/";
				});
			});
		});
	});
});

function updateHeight(){
	var height = window.innerHeight - 60 - 16;
	$("#side-menu").css("min-height", height + "px");
	$("#main").css("min-height", ($("#side-menu").height()) + "px");
}

function closeFlashMessage(objet){
	$(objet.parentNode).animate({
		height : "0px",
		opacity : "0",
		margin : "0",
		padding: "0"
	},
	250,
	function(){$(objet.parentNode).remove();});
}