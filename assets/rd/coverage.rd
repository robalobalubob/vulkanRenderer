# RD command coverage scene
#
# Exercises every geometry command that the portfolio generators do not emit,
# so the loader's newer paths get visual verification. Z-up, as the RD spec
# assumes (angular extents sweep clockwise around +Z starting from +X).
#
# Layout: three rows along X.
#   y = -5  swept surfaces      (Disk, Cylinder, Cone, Hyperboloid, Paraboloid)
#   y =  0  control-point forms (Patch, Curve, Subdivision) and object instances
#   y = +5  lines and points    (LineSet, PointSet, Line, Point, Circle)

Display "RD Coverage" "Screen" "rgbdouble"
Format 1280 800

CameraEye 0 -26 12
CameraAt 0 0 0
CameraUp 0 0 1
CameraFOV 40
Clipping 0.5 200
Background 0.04 0.05 0.09

# A parameterized object: $1 drives the colour, $2 the sphere radius.
ObjectBegin 2 "bead"
Color $1 0.35 0.85
Surface "plastic"
Ka 0.3
Kd 0.7
Ks 0.4
Specular 1 1 1 40
Sphere $2 -10 10 360
ObjectEnd

WorldBegin

AmbientLight 0.35 0.38 0.45 1.0
FarLight 0.4 -0.6 0.8  1.0 0.98 0.92  1.6
PointLight 6 -8 8  0.6 0.7 1.0  40.0

# ---------------------------------------------------------------------------
# Row 1 (y = -5): swept surfaces, including partial angular extents
# ---------------------------------------------------------------------------

Surface "plastic"
Ka 0.35
Kd 0.75
Ks 0.35
Specular 1 1 1 30

# Disk, full sweep
Color 0.9 0.45 0.25
XformPush
Translate -10 -5 0
Disk 0 2.0 360
XformPop

# Disk, 240 degree wedge
Color 0.95 0.7 0.3
XformPush
Translate -5 -5 0
Disk 0 2.0 240
XformPop

# Cylinder, 270 degree sweep (open tube, so the inner wall shows)
Color 0.4 0.8 0.5
XformPush
Translate 0 -5 -1.5
Cylinder 1.6 0 3 270
XformPop

# Cone, 300 degree sweep
Color 0.45 0.65 0.95
XformPush
Translate 5 -5 -1.5
Cone 3.5 1.6 300
XformPop

# Paraboloid
Color 0.8 0.5 0.9
XformPush
Translate 10 -5 -1.5
Paraboloid 1.8 0 3.2 360
XformPop

# ---------------------------------------------------------------------------
# Row 2 (y = 0): control-point surfaces, subdivision cage, object instances
# ---------------------------------------------------------------------------

# Hyperboloid: sweeping a skew line gives the waisted shell
Color 0.55 0.85 0.85
XformPush
Translate -10 0 -1.5
Hyperboloid 1.6 -1.2 0  1.6 1.2 3  360
XformPop

# Biquadratic Bezier patch with per-control-point colour
XformPush
Translate -5 0 0
Scale 2.2 2.2 2.2
Patch "Bezier" "PC"
2 2
-0.5 -0.5 -1.0   1.0 0.2 0.2
 0.0 -0.5 -0.5   1.0 1.0 0.2
 0.5 -0.5 -1.0   1.0 1.0 1.0
-0.7  0.0  0.0   1.0 0.2 1.0
 0.0  0.2 -1.0   0.5 0.5 0.5
 0.7  0.0  0.0   0.2 1.0 0.2
-0.5  0.5  0.0   1.0 1.0 1.0
 0.1  0.7  1.0   1.0 1.0 0.2
 0.5  0.5 -1.0   0.2 1.0 1.0
XformPop

# Cubic Bezier curve, drawn as an unlit polyline
XformPush
Translate 0 0 0
Scale 3.0 3.0 3.0
Curve "Bezier" "PC"
3
-0.5  0.0 -0.5   1.0 0.2 0.2
-0.5  0.0  1.0   1.0 1.0 0.2
 0.5  0.0 -1.0   1.0 1.0 1.0
 0.5  0.0  0.5   0.2 1.0 0.2
XformPop

# Subdivision control cage (rendered as its polygon cage)
Color 0.7 0.7 0.8
XformPush
Translate 5 0 0
Scale 1.4 1.4 1.4
Subdivision "Catmull-Clark" "P"
8 6 0
-1.0 -1.0 -1.0
 1.0 -1.0 -1.0
-1.0  1.0 -1.0
 1.0  1.0 -1.0
-1.0 -1.0  1.0
 1.0 -1.0  1.0
-1.0  1.0  1.0
 1.0  1.0  1.0
0 2 3 1 -1
0 1 5 4 -1
1 3 7 5 -1
3 2 6 7 -1
2 0 4 6 -1
4 5 7 6 -1
XformPop

# Two instances of the same parameterized object
XformPush
Translate 9 0 0
ObjectInstance "bead" 0.95 1.3
XformPop

XformPush
Translate 12 0 1.5
ObjectInstance "bead" 0.2 0.8
XformPop

# ---------------------------------------------------------------------------
# Row 3 (y = +5): lines, points, and the wireframe option
# ---------------------------------------------------------------------------

# LineSet: a colour cube drawn as six face loops
XformPush
Translate -10 5 0
Scale 1.6 1.6 1.6
LineSet "PC"
8 6
-1.0 -1.0 -1.0   0.3 0.3 0.3
 1.0 -1.0 -1.0   1.0 0.2 0.2
-1.0  1.0 -1.0   0.2 1.0 0.2
 1.0  1.0 -1.0   1.0 1.0 0.2
-1.0 -1.0  1.0   0.2 0.2 1.0
 1.0 -1.0  1.0   1.0 0.2 1.0
-1.0  1.0  1.0   0.2 1.0 1.0
 1.0  1.0  1.0   1.0 1.0 1.0
0 2 3 1 -1
0 1 5 4 -1
1 3 7 5 -1
3 2 6 7 -1
2 0 4 6 -1
4 5 7 6 -1
XformPop

# PointSet with per-point colour
XformPush
Translate -5 5 0
PointSet "PC"
9
-1.5 0.0 -1.5   1.0 0.3 0.3
 0.0 0.0 -1.5   1.0 0.7 0.3
 1.5 0.0 -1.5   1.0 1.0 0.3
-1.5 0.0  0.0   0.3 1.0 0.3
 0.0 0.0  0.0   1.0 1.0 1.0
 1.5 0.0  0.0   0.3 1.0 1.0
-1.5 0.0  1.5   0.3 0.3 1.0
 0.0 0.0  1.5   0.7 0.3 1.0
 1.5 0.0  1.5   1.0 0.3 1.0
XformPop

# Single Line and Point, both taking the current drawing colour
Color 1.0 0.85 0.2
XformPush
Translate 0 5 0
Line -1.8 0 -1.8   1.8 0 1.8
Line -1.8 0  1.8   1.8 0 -1.8
Point 0 0 0
XformPop

# Circle (drawn in the XY plane rather than screen-aligned)
Color 0.4 0.9 1.0
XformPush
Translate 5 5 0
Circle 0 0 0 2.0
XformPop

# Wireframe option applied to a solid primitive
Color 0.9 0.9 0.5
XformPush
Translate 10 5 0
OptionBool "Wireframe" true
Sphere 2.0 -2.0 2.0 360
OptionBool "Wireframe" false
XformPop

WorldEnd
