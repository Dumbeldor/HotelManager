extends StreamPlayer

var music1
var music2
var music3

func _ready():
	set_process(true)
	music1 = load("sounds/Gymnopedie No 1.ogg")
	music2 = load("sounds/Gymnopedie No 2.ogg")
	music3 = load("sounds/Gymnopedie No 3.ogg")

func _process(delta):
	if self.is_playing() != true:
		if (self.get_stream() == music1):
			self.set_stream(music2)
		elif self.get_stream() == music2:
			self.set_stream(music3)
		else:
			self.set_stream(music1)

		self.play()
		self.set_volume(get_node("/root/global").configFile.get_value("Sound", "music", 1.0))


