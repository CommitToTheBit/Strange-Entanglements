class_name QubitOrbit

extends Line2D

var phase : Polygon2D
var phase_spacing : Polygon2D

func _ready():
	phase = get_node("Phase")
	phase_spacing = phase.get_node("Spacing")
	set_quark_radius(1.2 * width);

func set_radius(radius : float):
	for i in range(points.size()):
		points[i] = radius * Vector2.RIGHT.rotated(2.0 * PI * float(i) / points.size())
	
	phase.position = radius * Vector2.RIGHT;

func set_quark_radius(radius : float):
	for i in range(phase.polygon.size()):
		phase.polygon[i] = radius * Vector2.RIGHT.rotated(2.0 * PI * float(i) / phase.polygon.size())
	for i in range(phase_spacing.polygon.size()):
		phase_spacing.polygon[i] = radius / 3.0 * Vector2.RIGHT.rotated(2.0 * PI * float(i) / phase_spacing.polygon.size())
