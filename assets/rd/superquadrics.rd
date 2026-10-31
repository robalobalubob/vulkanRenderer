# Superquadrics Demonstration Scene with Star and Sun Lighting
Display "Superquadrics Demo" "Screen" "rgbdouble"
Format 800 600
OptionReal "Divisions" 20

CameraEye 9 7 12
CameraAt 0 1 0
CameraUp 0 1 0
CameraFOV 45
Clipping 0.1 1000

Background 0.02 0.02 0.06

WorldBegin
# Ambient light for base illumination
AmbientLight 0.08 0.08 0.12 0.3

# Special ambient light to make sun visible
AmbientLight 0.3 0.3 0.2 0.5

# Main sun light source at the center
PointLight 0 0 0 1.0 1.0 0.7 12.0

# Generating star field with various star types
XformPush
    Translate -1.56266 2.55693 -12.5409
    Color 0.938265 0.938265 0.938265
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.044991 0.044991 0.044991
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 0.382877 -5.5148 7.04194
    Color 0.848895 0.848895 0.848895
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0745641 0.0745641 0.0745641
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 1.01865 -6.21369 -6.68095
    Color 0.875719 0.875719 0.875719
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0647591 0.0647591 0.0647591
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -0.381743 -0.0818687 -13.615
    Color 0.895407 0.895407 0.895407
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0793979 0.0793979 0.0793979
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 6.95048 -2.25877 8.99798
    Color 0.885631 0.885631 0.885631
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0783046 0.0783046 0.0783046
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -0.383113 -4.32081 -7.58567
    Color 0.806404 0.806404 0.806404
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0428521 0.0428521 0.0428521
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -0.974014 3.38573 -12.424
    Color 0.980033 0.980033 0.980033
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0757524 0.0757524 0.0757524
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -7.91755 0.475183 3.28427
    Color 0.786776 0.786776 0.786776
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0658806 0.0658806 0.0658806
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -5.13942 -0.678658 13.2909
    Color 0.890096 0.890096 0.890096
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0463145 0.0463145 0.0463145
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -0.44705 -13.1757 -5.37527
    Color 0.794854 0.794854 0.794854
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0488043 0.0488043 0.0488043
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 1.84926 0.277905 11.4477
    Color 0.769307 0.769307 0.769307
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0392192 0.0392192 0.0392192
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -5.45983 -3.51361 -6.28208
    Color 0.833128 0.833128 0.833128
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0300412 0.0300412 0.0300412
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -8.77613 1.43914 -8.32052
    Color 0.89934 0.89934 0.89934
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0356204 0.0356204 0.0356204
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -0.710193 0.264705 12.5542
    Color 0.724408 0.724408 0.724408
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0360392 0.0360392 0.0360392
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 4.28606 1.96381 -12.2008
    Color 0.738796 0.738796 0.738796
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0558171 0.0558171 0.0558171
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 10.6465 5.59715 2.56523
    Color 0.766842 0.766842 0.766842
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0672134 0.0672134 0.0672134
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 7.79293 7.66451 -2.52405
    Color 0.842615 0.842615 0.842615
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0474494 0.0474494 0.0474494
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 8.51875 -3.24595 4.57084
    Color 0.839015 0.839015 0.839015
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.064632 0.064632 0.064632
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -4.63779 -6.0608 -9.77001
    Color 0.781337 0.781337 0.781337
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0502229 0.0502229 0.0502229
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -9.38203 3.2472 10.6864
    Color 0.956937 0.956937 0.956937
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0391116 0.0391116 0.0391116
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 2.45638 2.86499 -9.54154
    Color 0.948242 0.948242 0.948242
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0387527 0.0387527 0.0387527
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 3.96524 1.9417 -12.4247
    Color 0.740353 0.740353 0.740353
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0535449 0.0535449 0.0535449
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 6.92862 -7.0299 4.33951
    Color 0.828415 0.828415 0.828415
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0550599 0.0550599 0.0550599
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 10.2768 -0.834344 5.37377
    Color 0.929117 0.929117 0.929117
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0660996 0.0660996 0.0660996
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -5.59478 -11.0006 7.46571
    Color 0.877156 0.877156 0.877156
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0724785 0.0724785 0.0724785
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -4.90682 -5.38266 -4.80238
    Color 0.860902 0.860902 0.860902
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.05186 0.05186 0.05186
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate 9.85325 -0.745232 -2.27147
    Color 0.949464 0.949464 0.949464
    Surface "matte"
    Ka 0.8
    Kd 0.9
    Ks 0
    Specular 0.8 0.8 0.8 10
    Scale 0.0434373 0.0434373 0.0434373
    Sphere 1.0 -1.0 1.0 360
XformPop

XformPush
    Translate -3.43892 6.15074 -5.77494
    Color 0.7 0.7 1
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 72.9347
    Rotate "Y" 14.8201
    Rotate "Z" 83.2649
    Scale 0.133584 0.133584 0.133584
    SqSphere 1 0.407833 0.438006 -1 1 360
XformPop

XformPush
    Translate -1.57126 6.46209 -4.39141
    Color 1 0.7 0.7
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 47.9196
    Rotate "Y" 86.4495
    Rotate "Z" 33.5042
    Scale 0.0848181 0.0848181 0.0848181
    SqSphere 1 0.294715 0.320925 -1 1 360
XformPop

XformPush
    Translate -1.70255 7.41143 -9.90552
    Color 1 1 0.8
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 52.3818
    Rotate "Y" 26.013
    Rotate "Z" 10.315
    Scale 0.140921 0.140921 0.140921
    SqSphere 1 0.317985 0.467493 -1 1 360
XformPop

XformPush
    Translate 9.0553 3.80762 1.05983
    Color 0.7 0.7 1
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 9.32807
    Rotate "Y" 29.4781
    Rotate "Z" 57.419
    Scale 0.0989884 0.0989884 0.0989884
    SqSphere 1 0.264123 0.328403 -1 1 360
XformPop

XformPush
    Translate 9.28598 -7.70841 2.55043
    Color 1 0.7 0.7
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 48.77
    Rotate "Y" 19.2229
    Rotate "Z" 50.9979
    Scale 0.0984428 0.0984428 0.0984428
    SqSphere 1 0.423494 0.338499 -1 1 360
XformPop

XformPush
    Translate 2.28112 3.0136 -9.70887
    Color 1 1 0.8
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 33.9731
    Rotate "Y" 75.255
    Rotate "Z" 10.3707
    Scale 0.0970688 0.0970688 0.0970688
    SqSphere 1 0.437611 0.409573 -1 1 360
XformPop

XformPush
    Translate -3.23587 7.40846 4.243
    Color 0.7 0.7 1
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 59.7297
    Rotate "Y" 52.5796
    Rotate "Z" 74.7132
    Scale 0.110198 0.110198 0.110198
    SqSphere 1 0.297991 0.373296 -1 1 360
XformPop

XformPush
    Translate 7.12663 0.744972 -5.21663
    Color 1 0.7 0.7
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Rotate "X" 61.798
    Rotate "Y" 16.684
    Rotate "Z" 20.9068
    Scale 0.133414 0.133414 0.133414
    SqSphere 1 0.344103 0.46058 -1 1 360
XformPop

# Light source at position 9.84698, 0, -4.21896
PointLight 9.84698 0 -4.21896 0.6 0.6 1 0.3

XformPush
    Translate 9.84698 0 -4.21896
    Color 0.6 0.6 1
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 0.25 0.25 0.25
    SqSphere 1 1 1 -1 1 360
XformPop

# Light source at position 3.17383, 9.76804, -0.141784
PointLight 3.17383 9.76804 -0.141784 1 0.6 0.6 0.3

XformPush
    Translate 3.17383 9.76804 -0.141784
    Color 1 0.6 0.6
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 0.25 0.25 0.25
    SqSphere 1 1 1 -1 1 360
XformPop

# Light source at position -6.83016, 4.9624, 5.11014
PointLight -6.83016 4.9624 5.11014 1 1 0.8 0.3

XformPush
    Translate -6.83016 4.9624 5.11014
    Color 1 1 0.8
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 0.25 0.25 0.25
    SqSphere 1 1 1 -1 1 360
XformPop

# Light source at position -7.44856, -5.4117, 0.956497
PointLight -7.44856 -5.4117 0.956497 0.6 0.6 1 0.3

XformPush
    Translate -7.44856 -5.4117 0.956497
    Color 0.6 0.6 1
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 0.25 0.25 0.25
    SqSphere 1 1 1 -1 1 360
XformPop

# Light source at position 3.08846, -9.50529, 1.71448
PointLight 3.08846 -9.50529 1.71448 1 0.6 0.6 0.3

XformPush
    Translate 3.08846 -9.50529 1.71448
    Color 1 0.6 0.6
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 0.25 0.25 0.25
    SqSphere 1 1 1 -1 1 360
XformPop

# Rendering celestial objects
# Light source at position 0, 0, 0
PointLight 0 0 0 1 1 0.7 12

XformPush
    Translate 0 0 0
    # Special emissive effect for the sun
    Color 1.0 0.9 0.4
    Surface "matte"
    Ka 1.0
    Kd 1.0
    Ks 0.3
    Specular 1.0 0.9 0.5 5
    SqSphere 1 0.3 0.3 -1 1 360
XformPop

XformPush
    Translate -3 0 3
    Color 0.4 0.4 0.8
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 0.8 0.8 0.8
    SqSphere 1 2 2 -1 1 360
XformPop

XformPush
    Translate 4 -1 -2
    Color 0.3 0.8 0.3
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 1.2 1.2 1.2
    SqSphere 1 0.5 2 -1 1 360
XformPop

XformPush
    Translate 5.5 0 -3
    Color 0.8 0.8 0.8
    Surface "plastic"
    Ka 0.3
    Kd 0.9
    Ks 0.5
    Specular 0.8 0.8 0.8 10
    Scale 0.4 0.4 0.4
    SqSphere 1 2 0.5 -1 1 360
XformPop

XformPush
    Translate -3 -1 -3
    Color 0.7 0.6 0.5
    Surface "plastic"
    Ka 0.2
    Kd 0.9
    Ks 0.3
    Specular 0.5 0.5 0.5 5
    Rotate "X" 15
    Rotate "Y" 20
    Scale 0.3 0.3 1
    SqSphere 1 0.3 0.3 -1 1 360
XformPop

# Rendering torus objects
XformPush
    Translate -4 2 -3
    Color 0.9 0.7 0.5
    Surface "plastic"
    Ka 0.2
    Kd 0.9
    Ks 0.4
    Specular 0.6 0.6 0.6 5
    Rotate "X" 75
    SqTorus 0.8 0.4 1 0.2 -180 180 360
XformPop

XformPush
    Translate 0 0 0
    Color 0.5 0.5 0.8
    Surface "plastic"
    Ka 0.15
    Kd 0.7
    Ks 0.3
    Specular 0.4 0.4 0.6 8
    Rotate "X" 30
    SqTorus 4 0.1 1 0.2 -180 180 360
XformPop

XformPush
    Translate 0 0 0
    Color 0.5 0.5 0.5
    Surface "plastic"
    Ka 0.15
    Kd 0.7
    Ks 0.3
    Specular 0.4 0.4 0.6 8
    Rotate "Y" 45
    SqTorus 5.5 0.2 2 2 -180 180 360
XformPop

WorldEnd
