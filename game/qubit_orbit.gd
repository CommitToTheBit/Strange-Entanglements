class_name QubitOrbit

extends Line2D

var quark : Polygon2D
var quark_spacing : Polygon2D

func _ready():
	quark = get_node("Quark")
	quark_spacing = get_node("Quark/Spacing")
	set_quark_radius(1.2 * width);

func set_radius(radius : float):
	for i in range(points.size()):
		points[i] = radius * Vector2.RIGHT.rotated(2.0 * PI * float(i) / points.size())
	
	quark.position = radius * Vector2.RIGHT;

func set_quark_radius(radius : float):
	for i in range(quark.polygon.size()):
		quark.polygon[i] = radius * Vector2.RIGHT.rotated(2.0 * PI * float(i) / quark.polygon.size())
	for i in range(quark_spacing.polygon.size()):
		quark_spacing.polygon[i] = radius / 3.0 * Vector2.RIGHT.rotated(2.0 * PI * float(i) / quark_spacing.polygon.size())
