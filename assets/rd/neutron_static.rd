# Neutron Tracks Visualization Scene
Display "Neutron Tracks Visualization" "Screen" "rgbsingle"
Format 800 600
OptionReal "Divisions" 20

CameraEye 0 15 15
CameraAt 0 0 0
CameraUp 0 1 0
CameraFOV 40
Clipping 0.1 1000

Background 0.05 0.05 0.12

WorldBegin
# Base ambient light to illuminate everything
AmbientLight 0.6 0.6 0.65 1.0
# Primary directional light
FarLight 1 1 1 1.0 1.0 1.0 1.5
# Point lights to highlight areas
PointLight 10 15 10 1.0 1.0 1.0 1.8
PointLight -10 15 -10 0.9 0.9 1.0 1.8
PointLight 0 15 0 1.0 1.0 0.9 2.0
# Source light at the center - increased intensity
PointLight 0 0 0 1.0 0.8 0.6 1.5

# Core region boundary - wireframe for transparency
XformPush
Color 1 0.3 0.3
Surface "metal"
Ka 1.0
Kd 0.7
Ks 1.0
Specular 1.0 1.0 1.0 20
OptionBool "Wireframe" true
Sphere 3 -3 3 360
OptionBool "Wireframe" false
XformPop

# Reflector region boundary - wireframe for transparency
XformPush
Color 0.3 0.7 1
Surface "metal"
Ka 0.7
Kd 0.0
Ks 1.0
Specular 1.0 1.0 1.0 15
OptionBool "Wireframe" true
Sphere 6 -6 6 360
OptionBool "Wireframe" false
XformPop

# Source point at center
XformPush
Color 1.0 0.9 0.5
Translate 0 0 0
Scale 0.3 0.3 0.3
Sphere 1 -1 1 360
XformPop

# Neutron track tubes (1094 tube segments)
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0945316 0.363263 -0.241871 0.294916 0.880879 -0.746271 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.294916 0.880879 -0.746271 0.356716 1.38173 -1.3011 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.356716 1.38173 -1.3011 0.385219 2.0214 -1.69162 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.385219 2.0214 -1.69162 0.572345 2.59631 -2.13542 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.572345 2.59631 -2.13542 0.682529 3.24607 -2.49343 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.682529 3.24607 -2.49343 0.836573 3.88964 -2.84641 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.836573 3.88964 -2.84641 0.88331 4.62165 -3.00286 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.88331 4.62165 -3.00286 0.916529 5.34089 -3.21282 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0121057 -0.0167093 0.400295 -0.436238 0.0598639 1.01409 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.436238 0.0598639 1.01409 -0.997502 0.12774 1.50692 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.997502 0.12774 1.50692 -1.6145 -0.0658609 1.88683 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.6145 -0.0658609 1.88683 -2.35031 0.0241191 2.00079 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.00787726 -0.439578 0.12648 -0.172673 -1.15876 0.0138458 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.172673 -1.15876 0.0138458 -0.376097 -1.85215 -0.186951 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.376097 -1.85215 -0.186951 -0.381594 -2.55343 -0.452816 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.381594 -2.55343 -0.452816 -0.681485 -2.96507 -1.00338 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.681485 -2.96507 -1.00338 -0.849778 -3.40501 -1.58701 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.849778 -3.40501 -1.58701 -0.79562 -3.73567 -2.258 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0458913 -0.486879 -0.0316927 -0.0461872 -1.16945 -0.342482 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0461872 -1.16945 -0.342482 0.00468569 -1.82479 -0.70365 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.00468569 -1.82479 -0.70365 -0.0731031 -2.4695 -1.07889 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0731031 -2.4695 -1.07889 0.120168 -2.90197 -1.66037 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.120168 -2.90197 -1.66037 0.414906 -3.27066 -2.2432 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0104681 0.0169079 -0.125462 0.00572769 0.16538 -0.860604 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0727948 -0.101254 -0.467914 -0.12717 -0.00564475 -1.2098 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.12717 -0.00564475 -1.2098 -0.328031 0.396005 -1.8105 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.328031 0.396005 -1.8105 -0.384637 0.921416 -2.3427 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.384637 0.921416 -2.3427 -0.249899 1.38276 -2.91847 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.094614 -0.102234 0.120371 -0.363776 -0.392205 0.757528 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.363776 -0.392205 0.757528 -0.824517 -0.872302 1.10354 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.824517 -0.872302 1.10354 -1.00787 -1.39637 1.60775 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.00787 -1.39637 1.60775 -1.00288 -2.10705 1.84736 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.00288 -2.10705 1.84736 -0.845159 -2.69583 2.28436 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.167744 -0.352507 -0.187912 0.768722 -0.800861 -0.170453 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.768722 -0.800861 -0.170453 1.25619 -1.34706 -0.33338 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.25619 -1.34706 -0.33338 1.80144 -1.85773 -0.399835 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.80144 -1.85773 -0.399835 2.31332 -2.40512 -0.370784 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.104211 -0.223109 0.162353 0.211776 -0.901579 0.463366 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.211776 -0.901579 0.463366 0.540746 -1.43667 0.873189 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.540746 -1.43667 0.873189 0.892103 -1.84973 1.39129 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.892103 -1.84973 1.39129 0.94798 -2.19175 2.05643 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.101258 -0.0667691 0.288067 -0.0275368 -0.385334 0.954721 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0275368 -0.385334 0.954721 -0.360345 -0.581486 1.59758 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.360345 -0.581486 1.59758 -0.741925 -1.01584 2.07532 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.741925 -1.01584 2.07532 -1.18393 -1.50021 2.43935 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.18393 -1.50021 2.43935 -1.67675 -1.88056 2.85763 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0762248 0.145408 0.341622 0.267627 0.428839 1.0091 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0384371 0.163604 -0.0832636 -0.00832849 0.74574 -0.555186 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.00832849 0.74574 -0.555186 -0.230453 1.34717 -0.944349 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.230453 1.34717 -0.944349 -0.299696 1.91366 -1.43095 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.299696 1.91366 -1.43095 -0.342212 2.41037 -1.99129 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.342212 2.41037 -1.99129 -0.306376 2.63386 -2.70632 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.306376 2.63386 -2.70632 -0.479946 3.13514 -3.23649 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.479946 3.13514 -3.23649 -0.768188 3.51834 -3.81319 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.141514 0.046167 0.103017 0.627546 0.0416891 0.674202 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.627546 0.0416891 0.674202 1.00585 0.190113 1.30457 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.00585 0.190113 1.30457 1.41196 0.328463 1.91973 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.41196 0.328463 1.91973 1.99048 0.65039 2.27212 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.99048 0.65039 2.27212 2.47353 0.859861 2.80625 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.47353 0.859861 2.80625 2.99125 1.25659 3.17648 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.99125 1.25659 3.17648 3.3045 1.50569 3.81077 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.3045 1.50569 3.81077 3.78324 1.92965 4.20264 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.78324 1.92965 4.20264 4.16898 2.37087 4.67065 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0529846 -0.062659 -0.462522 0.222193 -0.0248755 -1.19221 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.222193 -0.0248755 -1.19221 0.235081 0.0350787 -1.9397 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.235081 0.0350787 -1.9397 0.256608 -0.0279016 -2.68674 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.256608 -0.0279016 -2.68674 0.391485 0.0690841 -3.41811 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.391485 0.0690841 -3.41811 0.605688 0.325616 -4.08953 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.605688 0.325616 -4.08953 0.929519 0.378579 -4.76394 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.455482 0.159612 -0.0558424 -1.16521 0.400555 -0.0288537 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.16521 0.400555 -0.0288537 -1.80382 0.744072 0.162667 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.80382 0.744072 0.162667 -2.47966 1.00412 0.357908 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.47966 1.00412 0.357908 -3.09799 1.25121 0.703029 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.313594 -0.251444 -0.216132 0.934025 -0.643989 -0.369346 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.934025 -0.643989 -0.369346 1.65146 -0.861233 -0.393778 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.65146 -0.861233 -0.393778 2.39965 -0.872965 -0.444499 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.39965 -0.872965 -0.444499 3.06082 -0.997674 -0.775863 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.06082 -0.997674 -0.775863 3.67303 -1.18599 -1.16604 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.67303 -1.18599 -1.16604 4.07787 -1.28607 -1.78941 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.184164 -0.0941269 0.315344 -0.590654 -0.21447 0.934039 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.590654 -0.21447 0.934039 -0.921612 -0.396893 1.58187 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.921612 -0.396893 1.58187 -1.27222 -0.726255 2.15728 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.27222 -0.726255 2.15728 -1.3848 -1.38449 2.49867 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.214931 -0.0180054 0.0675258 -0.657964 -0.188887 0.648061 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.657964 -0.188887 0.648061 -1.09746 -0.35985 1.23126 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.369046 -0.104471 -0.223573 -0.966611 -0.086393 -0.676442 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.267714 -0.13282 0.18978 -0.762989 -0.50774 0.610063 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.762989 -0.50774 0.610063 -1.42449 -0.614097 0.94711 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.42449 -0.614097 0.94711 -2.0353 -0.515225 1.37095 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.0353 -0.515225 1.37095 -2.76938 -0.572152 1.51371 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0451506 0.337545 -0.126804 0.473709 0.946922 -0.0402118 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.473709 0.946922 -0.0402118 0.886604 1.52358 -0.284101 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.886604 1.52358 -0.284101 1.39907 2.03258 -0.486083 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.39907 2.03258 -0.486083 1.84614 2.60307 -0.678877 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.84614 2.60307 -0.678877 2.35237 3.15459 -0.63345 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.463557 0.0638582 0.128239 -1.13205 0.170691 0.45104 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.13205 0.170691 0.45104 -1.83815 0.388961 0.578682 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.83815 0.388961 0.578682 -2.51713 0.383688 0.89719 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.231089 -0.097695 0.324816 -0.507662 -0.186328 1.0163 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.19522 -0.0513181 0.201106 -0.822798 0.0722236 0.592747 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.00267621 -0.289859 -0.089959 0.0588695 -1.03459 -0.153908 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0588695 -1.03459 -0.153908 -0.0315568 -1.72875 -0.423101 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0315568 -1.72875 -0.423101 -0.157358 -2.4623 -0.515728 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.157358 -2.4623 -0.515728 -0.355431 -3.18411 -0.468254 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.217723 -0.12273 0.317844 -0.882111 -0.316663 0.606771 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.882111 -0.316663 0.606771 -1.62776 -0.236233 0.600878 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.62776 -0.236233 0.600878 -2.36602 -0.213419 0.470645 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.36602 -0.213419 0.470645 -3.00125 -0.402937 0.119843 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.00125 -0.402937 0.119843 -3.65598 -0.67502 -0.124695 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.65598 -0.67502 -0.124695 -4.0437 -1.08785 -0.616377 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.186965 -0.362089 0.0296616 0.401555 -0.988955 -0.32175 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.401555 -0.988955 -0.32175 0.485076 -1.712 -0.502685 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.42237 0.136668 -0.0352368 1.03743 0.246892 -0.450028 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.03743 0.246892 -0.450028 1.71616 0.322184 -0.760122 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.71616 0.322184 -0.760122 2.46414 0.271282 -0.780802 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.46414 0.271282 -0.780802 3.20413 0.156124 -0.821507 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.20413 0.156124 -0.821507 3.90177 -0.103801 -0.730775 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.148202 -0.0666275 -0.463452 0.0266208 -0.130432 -1.19 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0266208 -0.130432 -1.19 0.0985948 0.0338821 -1.91823 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.106457 -0.284458 -0.319063 0.0333464 -0.939542 -0.676863 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0333464 -0.939542 -0.676863 0.118112 -1.65411 -0.888319 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.118112 -1.65411 -0.888319 0.122033 -2.40309 -0.927075 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.122033 -2.40309 -0.927075 0.228147 -3.10945 -1.15576 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.228147 -3.10945 -1.15576 0.209268 -3.80815 -1.42773 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.399383 0.0858171 0.102402 -1.05283 0.384273 0.317883 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.05283 0.384273 0.317883 -1.76332 0.508838 0.523282 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.76332 0.508838 0.523282 -2.43431 0.780614 0.719257 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.43431 0.780614 0.719257 -3.01087 1.25818 0.764107 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.01087 1.25818 0.764107 -3.60582 1.7012 0.653341 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.60582 1.7012 0.653341 -4.29031 1.98551 0.768001 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -4.29031 1.98551 0.768001 -5.01664 1.97265 0.954455 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -5.01664 1.97265 0.954455 -5.60164 1.71862 1.34911 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.248404 -0.131354 0.271905 0.504227 -0.751562 0.607162 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.504227 -0.751562 0.607162 0.479046 -1.47532 0.802209 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.479046 -1.47532 0.802209 0.611797 -2.20373 0.921783 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.611797 -2.20373 0.921783 0.664921 -2.91494 1.15384 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.664921 -2.91494 1.15384 0.529234 -3.62597 1.35012 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.00291819 0.175999 0.263042 0.033074 0.533522 0.921359 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.033074 0.533522 0.921359 -0.11676 0.957774 1.52141 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.11676 0.957774 1.52141 -0.072867 1.26015 2.20635 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.072867 1.26015 2.20635 0.159588 1.45436 2.89246 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.304717 0.154994 0.0697017 0.892283 0.613969 0.150992 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.892283 0.613969 0.150992 1.36015 1.18916 0.26391 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.36015 1.18916 0.26391 1.78576 1.79943 0.358458 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.78576 1.79943 0.358458 2.23815 2.3976 0.363685 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.23815 2.3976 0.363685 2.56029 3.01737 0.636834 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0393282 0.00993292 0.289628 0.119212 -0.236898 0.993327 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.119212 -0.236898 0.993327 0.238218 -0.189959 1.73234 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.238218 -0.189959 1.73234 0.296972 -0.226587 2.47913 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.296972 -0.226587 2.47913 0.0734275 -0.125724 3.1879 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0734275 -0.125724 3.1879 -0.271692 0.0240703 3.83671 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.271692 0.0240703 3.83671 -0.619431 0.218368 4.47219 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.619431 0.218368 4.47219 -1.10855 0.598308 4.89516 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0555931 0.176317 -0.00344662 -0.0831814 0.923852 0.0506944 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0831814 0.923852 0.0506944 0.142102 1.60154 -0.178409 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.142102 1.60154 -0.178409 0.170322 2.26798 -0.521273 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.170322 2.26798 -0.521273 0.135441 2.81356 -1.03472 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.135441 2.81356 -1.03472 0.142996 3.40323 -1.49811 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.151346 0.281338 -0.355943 -0.270939 0.802512 -0.881846 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.270939 0.802512 -0.881846 -0.328818 1.36993 -1.36886 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.328818 1.36993 -1.36886 -0.218944 1.68951 -2.03841 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0498717 0.21662 -0.316613 0.143498 0.652885 -0.919445 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.143498 0.652885 -0.919445 0.126763 1.12768 -1.49978 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.126763 1.12768 -1.49978 -0.0887304 1.4776 -2.12717 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0887304 1.4776 -2.12717 -0.24721 1.93638 -2.69893 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0564754 -0.184341 0.0946599 0.263006 -0.827347 0.420832 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.263006 -0.827347 0.420832 0.515382 -1.49265 0.65786 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.515382 -1.49265 0.65786 0.567593 -2.24068 0.672661 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.567593 -2.24068 0.672661 0.780622 -2.9548 0.757204 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.780622 -2.9548 0.757204 0.917475 -3.69111 0.716833 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.917475 -3.69111 0.716833 0.997024 -4.43532 0.764941 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.997024 -4.43532 0.764941 1.26469 -5.11638 0.929299 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.26469 -5.11638 0.929299 1.76718 -5.6562 0.792922 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.76718 -5.6562 0.792922 2.21441 -6.25363 0.718345 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.121453 -0.115822 0.246979 -0.427479 -0.141617 0.931217 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.427479 -0.141617 0.931217 -0.736077 0.0848268 1.57619 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.736077 0.0848268 1.57619 -1.32411 0.0357074 2.03912 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.32411 0.0357074 2.03912 -1.87352 -0.103421 2.53033 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.87352 -0.103421 2.53033 -2.34565 -0.323392 3.06997 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.34565 -0.323392 3.06997 -2.59675 -0.726913 3.65016 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.59675 -0.726913 3.65016 -2.80942 -1.18933 4.20102 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.80942 -1.18933 4.20102 -3.25468 -1.73698 4.45463 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.25468 -1.73698 4.45463 -3.89087 -2.09324 4.63025 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.89087 -2.09324 4.63025 -4.57428 -2.40063 4.66123 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.192255 0.29637 -0.225621 -0.616658 0.838367 -0.523316 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.616658 0.838367 -0.523316 -1.13196 1.37485 -0.618967 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.13196 1.37485 -0.618967 -1.51939 1.86502 -1.03385 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.51939 1.86502 -1.03385 -1.75627 2.31848 -1.58227 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.75627 2.31848 -1.58227 -1.75564 2.49491 -2.31122 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.75564 2.49491 -2.31122 -1.96168 2.43826 -3.03014 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.184896 -0.0680561 -0.297105 -0.809632 -0.219952 -0.68328 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.809632 -0.219952 -0.68328 -1.41413 -0.168115 -1.12419 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.41413 -0.168115 -1.12419 -2.01906 -0.452557 -1.46426 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.199247 -0.156467 0.0653176 0.806036 -0.593066 0.00458701 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.806036 -0.593066 0.00458701 1.41669 -0.961362 0.236892 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.41669 -0.961362 0.236892 1.99247 -1.41648 0.391297 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.99247 -1.41648 0.391297 2.61311 -1.77622 0.610131 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.24536 0.146537 0.0840908 0.599862 0.485369 0.65156 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.599862 0.485369 0.65156 1.08192 0.954492 0.983288 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.08192 0.954492 0.983288 1.72598 1.30285 1.14558 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.72598 1.30285 1.14558 2.37251 1.64798 1.30488 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.295547 0.209058 -0.341437 0.503031 0.60202 -0.945615 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.503031 0.60202 -0.945615 0.692652 0.684199 -1.66658 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.692652 0.684199 -1.66658 0.96549 0.502807 -2.34123 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.257468 0.369687 -0.0175708 0.796007 0.695916 -0.425064 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.796007 0.695916 -0.425064 1.36784 0.721293 -0.909694 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.36784 0.721293 -0.909694 1.90856 0.711993 -1.42934 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.250137 0.0650496 0.106054 0.922956 0.171293 0.419944 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.922956 0.171293 0.419944 1.54319 0.393654 0.77823 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.54319 0.393654 0.77823 1.99488 0.379734 1.37679 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.99488 0.379734 1.37679 2.55529 0.193669 1.8392 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.108932 -0.330613 -0.153902 0.329634 -0.990779 -0.433134 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.329634 -0.990779 -0.433134 0.825598 -1.46024 -0.74318 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.825598 -1.46024 -0.74318 1.10714 -1.97924 -1.20564 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.10714 -1.97924 -1.20564 1.24758 -2.58099 -1.63069 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.152412 0.241453 0.253313 0.579245 0.759997 0.587119 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.579245 0.759997 0.587119 0.997707 1.16913 1.05616 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.997707 1.16913 1.05616 1.44954 1.66914 1.3853 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.44954 1.66914 1.3853 1.78694 2.31801 1.55152 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.78694 2.31801 1.55152 2.24174 2.90488 1.65755 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.24174 2.90488 1.65755 2.72489 3.45592 1.817 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.72489 3.45592 1.817 3.17647 4.05445 1.83509 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.317103 0.137344 -0.0351568 1.00684 0.350122 0.168536 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.00684 0.350122 0.168536 1.7063 0.465383 0.413417 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.7063 0.465383 0.413417 2.25496 0.254609 0.879301 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.25496 0.254609 0.879301 2.82093 -0.0298777 1.28085 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.150819 0.106416 0.300218 -0.423509 0.270231 0.979413 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.423509 0.270231 0.979413 -0.906061 0.431793 1.53036 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.906061 0.431793 1.53036 -1.41165 0.499889 2.08012 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.171852 0.330024 0.0832412 0.436905 1.02623 -0.00359322 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.436905 1.02623 -0.00359322 0.500009 1.74489 -0.208643 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.500009 1.74489 -0.208643 0.639382 2.39516 -0.555386 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.639382 2.39516 -0.555386 0.530042 2.92287 -1.07698 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.530042 2.92287 -1.07698 0.535752 3.32441 -1.71041 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.23691 0.285275 0.0717091 -0.861612 0.478358 0.439086 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.861612 0.478358 0.439086 -1.31824 0.978408 0.76149 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.169819 -0.0114952 0.0422959 0.843786 -0.329515 0.126744 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.843786 -0.329515 0.126744 1.50392 -0.58835 0.371129 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.50392 -0.58835 0.371129 2.09765 -0.817805 0.767794 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.09765 -0.817805 0.767794 2.52056 -1.11974 1.30861 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.52056 -1.11974 1.30861 2.79118 -1.52135 1.8813 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0616405 -0.376195 -0.156964 0.31195 -0.909099 -0.621571 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.31195 -0.909099 -0.621571 0.505682 -1.59085 -0.866901 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.505682 -1.59085 -0.866901 0.950435 -2.19371 -0.902243 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.950435 -2.19371 -0.902243 1.47954 -2.69691 -0.730949 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.105927 0.105308 0.23886 -0.219136 0.570924 0.815822 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.219136 0.570924 0.815822 -0.0251926 0.811405 1.49924 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0251926 0.811405 1.49924 0.00431643 0.891149 2.2444 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.00431643 0.891149 2.2444 -0.201433 0.843969 2.96408 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.201512 0.203457 -0.138064 -0.680355 0.742324 -0.345022 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.680355 0.742324 -0.345022 -1.16677 1.1183 -0.774605 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.16677 1.1183 -0.774605 -1.52516 1.73997 -0.992733 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.52516 1.73997 -0.992733 -1.68812 2.39446 -1.32073 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.304885 0.0340264 0.150742 -0.90757 -0.0943308 0.578287 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.90757 -0.0943308 0.578287 -1.59192 0.0377345 0.855287 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.59192 0.0377345 0.855287 -2.33597 0.10923 0.916667 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.33597 0.10923 0.916667 -2.93586 0.08162 1.36598 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.93586 0.08162 1.36598 -3.30722 -0.221999 1.94252 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.359847 0.07019 0.316182 -0.869315 0.377994 0.77247 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.869315 0.377994 0.77247 -1.54226 0.617929 1.00069 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.54226 0.617929 1.00069 -2.06229 0.66604 1.53896 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.06229 0.66604 1.53896 -2.63977 0.776879 2.00451 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0987396 0.0847587 -0.353295 -0.52605 0.486495 -0.82075 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.52605 0.486495 -0.82075 -0.734608 0.980043 -1.34555 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.734608 0.980043 -1.34555 -1.27109 1.24443 -1.79808 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.27109 1.24443 -1.79808 -1.73634 1.48698 -2.334 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.73634 1.48698 -2.334 -2.12782 1.73897 -2.922 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.12782 1.73897 -2.922 -2.26557 1.92614 -3.63509 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.26557 1.92614 -3.63509 -2.32871 2.13115 -4.35376 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.32871 2.13115 -4.35376 -2.13609 2.58113 -4.92201 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0677159 -0.19485 0.09395 0.389184 -0.807802 0.382824 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.389184 -0.807802 0.382824 0.945691 -1.29781 0.495481 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.945691 -1.29781 0.495481 1.23077 -1.90865 0.824271 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.23077 -1.90865 0.824271 1.60543 -2.41366 1.23304 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.60543 -2.41366 1.23304 2.10137 -2.81729 1.62499 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0983082 -0.100863 0.425167 0.0500082 -0.355464 1.11486 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0500082 -0.355464 1.11486 0.0323579 -0.480739 1.85412 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0323579 -0.480739 1.85412 0.000703011 -0.711287 2.5671 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.000703011 -0.711287 2.5671 -0.116804 -0.921308 3.27744 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.116804 -0.921308 3.27744 -0.357928 -1.21177 3.9255 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.357928 -1.21177 3.9255 -0.32414 -1.60296 4.56451 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.32414 -1.60296 4.56451 -0.250077 -1.85005 5.26876 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.250077 -1.85005 5.26876 -0.481143 -2.16084 5.91104 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.232405 -0.279147 0.267817 -0.836332 -0.683514 0.452908 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.836332 -0.683514 0.452908 -1.39141 -0.94653 0.88327 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.39141 -0.94653 0.88327 -1.9601 -1.30734 1.21328 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.9601 -1.30734 1.21328 -2.43851 -1.6365 1.68791 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.213868 0.341367 -0.128764 0.726684 0.887133 -0.169505 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.726684 0.887133 -0.169505 1.35762 1.22701 -0.390654 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.35762 1.22701 -0.390654 2.01574 1.52396 -0.593623 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.01574 1.52396 -0.593623 2.73681 1.63728 -0.766005 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.341618 0.172167 0.0278628 0.740063 0.801283 0.117053 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.740063 0.801283 0.117053 1.23525 1.329 -0.0799471 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.23525 1.329 -0.0799471 1.71092 1.86412 -0.30328 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.71092 1.86412 -0.30328 2.16816 2.44699 -0.420296 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.16816 2.44699 -0.420296 2.4734 3.11948 -0.289568 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.4734 3.11948 -0.289568 2.83472 3.73951 -0.071593 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.83472 3.73951 -0.071593 3.2836 4.30348 0.135618 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.2836 4.30348 0.135618 3.4977 4.9873 0.357117 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.110281 -0.000986623 -0.144732 0.534054 -0.140704 -0.747555 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.534054 -0.140704 -0.747555 0.858358 -0.12894 -1.42371 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.858358 -0.12894 -1.42371 1.42644 0.0102431 -1.89319 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.42644 0.0102431 -1.89319 1.91213 0.267626 -2.40345 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.91213 0.267626 -2.40345 2.3493 0.575026 -2.92965 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0225962 -0.0209175 -0.0945276 0.121486 -0.100972 -0.833657 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.121486 -0.100972 -0.833657 0.0682149 -0.0401474 -1.57929 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0682149 -0.0401474 -1.57929 -0.317646 0.216006 -2.1692 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.373505 -0.165256 -0.271581 1.09053 -0.318978 -0.428894 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.09053 -0.318978 -0.428894 1.81037 -0.333684 -0.638909 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.81037 -0.333684 -0.638909 2.5116 -0.58592 -0.554331 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.5116 -0.58592 -0.554331 3.1286 -1.01059 -0.516023 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.1286 -1.01059 -0.516023 3.71583 -1.44147 -0.694932 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.317115 -0.176428 -0.318157 0.886759 -0.209282 -0.804907 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.886759 -0.209282 -0.804907 1.45343 -0.300939 -1.28759 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.45343 -0.300939 -1.28759 1.91695 -0.54628 -1.82374 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.91695 -0.54628 -1.82374 2.18758 -0.576411 -2.52256 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.000247488 -0.385895 0.132056 -0.0474574 -1.112 0.313823 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0474574 -1.112 0.313823 -0.0105088 -1.77996 0.652888 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0105088 -1.77996 0.652888 -0.0846446 -2.41869 1.03893 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0846446 -2.41869 1.03893 -0.189919 -2.8269 1.65924 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.413723 0.0132312 0.104842 -1.01802 0.266866 0.469532 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.01802 0.266866 0.469532 -1.62804 0.510865 0.831245 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.62804 0.510865 0.831245 -2.34155 0.727726 0.911045 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.34155 0.727726 0.911045 -3.02688 0.956095 0.709352 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.02688 0.956095 0.709352 -3.68141 1.24084 0.479139 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.68141 1.24084 0.479139 -4.40427 1.25634 0.279794 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.144936 0.10625 0.256588 -0.557623 0.432101 0.791388 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.557623 0.432101 0.791388 -0.909294 1.01738 1.10167 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.909294 1.01738 1.10167 -1.55048 1.40018 1.17129 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.55048 1.40018 1.17129 -2.26046 1.57759 1.33547 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.176286 0.361201 0.0109056 0.188931 0.955801 0.467843 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.188931 0.955801 0.467843 0.147997 1.48156 1.00113 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.147997 1.48156 1.00113 0.257139 2.02868 1.50239 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.257139 2.02868 1.50239 0.240292 2.33503 2.18676 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.162699 -0.141787 0.0834796 0.616288 -0.68478 0.332308 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.616288 -0.68478 0.332308 1.1801 -1.07158 0.640537 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.1801 -1.07158 0.640537 1.79121 -1.22586 1.04703 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.79121 -1.22586 1.04703 2.36563 -1.61658 1.32966 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.405619 -0.108818 -0.196778 -0.815268 -0.142808 -0.8241 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.815268 -0.142808 -0.8241 -1.12616 0.11503 -1.45605 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.227097 0.019531 -0.172329 -0.854363 0.111492 -0.573053 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.854363 0.111492 -0.573053 -1.54097 0.323761 -0.787566 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.54097 0.323761 -0.787566 -2.10688 0.598885 -1.19568 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.10688 0.598885 -1.19568 -2.8427 0.629276 -1.33761 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0980771 -0.42503 0.0980751 -0.148053 -1.11185 0.271842 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.148053 -1.11185 0.271842 -0.669925 -1.64712 0.211496 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.669925 -1.64712 0.211496 -1.28586 -2.07415 0.18369 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.28586 -2.07415 0.18369 -1.97138 -2.37792 0.166987 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.407394 0.215329 0.106601 -0.910035 0.590644 0.517685 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.910035 0.590644 0.517685 -1.57298 0.906405 0.670309 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.330635 0.217662 0.227268 -1.01759 0.266846 0.524201 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.01759 0.266846 0.524201 -1.66839 0.484041 0.827167 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.66839 0.484041 0.827167 -2.29377 0.716584 1.16969 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.29377 0.716584 1.16969 -2.79629 1.12872 1.54401 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.79629 1.12872 1.54401 -3.47033 1.40609 1.72075 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.190373 -0.148811 0.286977 0.475627 -0.141018 0.980569 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.475627 -0.141018 0.980569 0.965203 -0.0979167 1.5471 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.965203 -0.0979167 1.5471 1.5685 -0.132319 1.99135 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.5685 -0.132319 1.99135 2.06402 -0.0263004 2.54426 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0896163 -0.294041 0.342401 -0.348486 -0.836704 0.790735 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.348486 -0.836704 0.790735 -0.36225 -1.50181 1.13706 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.36225 -1.50181 1.13706 0.0190248 -2.10708 1.3624 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0190248 -2.10708 1.3624 0.605896 -2.47872 1.64518 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.605896 -2.47872 1.64518 1.26782 -2.78623 1.8178 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.26782 -2.78623 1.8178 1.84054 -3.14125 2.14712 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.84054 -3.14125 2.14712 2.32277 -3.57769 2.52058 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.32277 -3.57769 2.52058 2.63809 -4.007 3.04857 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.63809 -4.007 3.04857 3.03554 -4.4855 3.46758 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0836273 -0.00680536 -0.468641 -0.127945 0.143893 -1.20201 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.127945 0.143893 -1.20201 -0.350422 0.413991 -1.86537 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.314169 -0.211912 0.0880221 0.952632 -0.501001 0.35503 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.952632 -0.501001 0.35503 1.48523 -0.776741 0.80537 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.48523 -0.776741 0.80537 1.93033 -0.808261 1.40819 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.93033 -0.808261 1.40819 2.34381 -1.03338 1.99201 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.34381 -1.03338 1.99201 2.84941 -1.25666 2.49899 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.84941 -1.25666 2.49899 3.25588 -1.73909 2.90463 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0851418 0.176915 0.455324 0.222679 0.22625 1.19095 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.222679 0.22625 1.19095 0.334103 -0.068497 1.87155 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.334103 -0.068497 1.87155 0.497773 -0.36409 2.54113 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.497773 -0.36409 2.54113 0.565926 -0.79949 3.14799 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.565926 -0.79949 3.14799 0.64254 -1.27429 3.72348 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.64254 -1.27429 3.72348 0.918621 -1.82938 4.14557 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.186939 -0.310025 0.252135 -0.563785 -0.703907 0.76725 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0808892 -0.295644 0.111008 0.456594 -0.778048 0.545326 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.456594 -0.778048 0.545326 0.933152 -1.03413 1.06476 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.933152 -1.03413 1.06476 1.33825 -1.46611 1.52497 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.33825 -1.46611 1.52497 1.78114 -1.7262 2.07151 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.78114 -1.7262 2.07151 2.05858 -1.9015 2.74589 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.173891 -0.285366 0.0875172 -0.567794 -0.923599 0.0874021 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.567794 -0.923599 0.0874021 -0.686771 -1.66403 0.0768543 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.686771 -1.66403 0.0768543 -0.725694 -2.37466 -0.159761 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.196717 0.182885 0.158218 0.540966 0.59681 0.680385 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.456676 -0.173124 0.00805441 1.11839 -0.499301 0.143138 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.11839 -0.499301 0.143138 1.70636 -0.961276 0.201132 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.70636 -0.961276 0.201132 2.30798 -1.37325 0.0255171 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.30798 -1.37325 0.0255171 2.90843 -1.80448 -0.10097 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.90843 -1.80448 -0.10097 3.39261 -2.37682 -0.0789848 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.168575 -0.228052 0.181133 0.30327 -0.576349 0.831553 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.30327 -0.576349 0.831553 0.189101 -1.08657 1.36927 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.189101 -1.08657 1.36927 -0.0595101 -1.552 1.90225 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0595101 -1.552 1.90225 -0.502241 -2.11949 2.11306 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.502241 -2.11949 2.11306 -0.98189 -2.69326 2.16988 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.31013 -0.206013 -0.106229 -0.770096 -0.520313 -0.608371 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.349425 -0.0047548 0.0453453 -1.03026 0.16779 0.308394 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.03026 0.16779 0.308394 -1.65672 0.444826 0.613834 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.65672 0.444826 0.613834 -2.39579 0.572291 0.619888 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.39579 0.572291 0.619888 -3.13512 0.557481 0.745085 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.411581 0.260057 0.0454222 0.953416 0.739917 0.242018 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.333272 -0.244398 -0.0141657 0.77692 -0.848015 0.0222125 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.77692 -0.848015 0.0222125 1.37018 -1.3057 -0.0105146 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.37018 -1.3057 -0.0105146 1.91433 -1.78886 -0.192038 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0154934 -0.304421 0.187468 -0.134058 -0.841056 0.697825 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.134058 -0.841056 0.697825 -0.331152 -1.18204 1.33609 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.331152 -1.18204 1.33609 -0.479623 -1.65775 1.89659 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.479623 -1.65775 1.89659 -0.481785 -2.07149 2.52214 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0612139 -0.059941 -0.348366 -0.27991 -0.0888755 -1.06519 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.27991 -0.0888755 -1.06519 -0.343866 -0.313482 -1.7779 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.343866 -0.313482 -1.7779 -0.353051 -0.655875 -2.44512 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.353051 -0.655875 -2.44512 -0.684575 -0.853981 -3.08804 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.274294 0.0679806 0.0196556 0.948441 0.320666 0.229836 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.948441 0.320666 0.229836 1.63773 0.412191 0.510915 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.63773 0.412191 0.510915 2.16003 0.706894 0.9613 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.16003 0.706894 0.9613 2.66973 0.884499 1.48204 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.66973 0.884499 1.48204 3.16817 1.0124 2.02766 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.366645 0.138637 0.281049 1.05966 0.317709 0.505027 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.05966 0.317709 0.505027 1.58169 0.686322 0.897595 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.58169 0.686322 0.897595 2.08954 1.1917 1.11938 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.08954 1.1917 1.11938 2.65178 1.6863 1.16122 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.65178 1.6863 1.16122 3.34408 1.95685 1.06107 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.34408 1.95685 1.06107 4.0806 1.99789 1.19653 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 4.0806 1.99789 1.19653 4.76578 1.75166 1.01654 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 4.76578 1.75166 1.01654 5.29331 1.52263 0.535122 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0561835 -0.0325481 0.406249 0.511612 -0.245015 0.962973 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.511612 -0.245015 0.962973 0.998209 -0.383701 1.51659 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.343952 -0.217253 -0.257586 0.881536 -0.530569 -0.676318 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.881536 -0.530569 -0.676318 1.43336 -0.553705 -1.18372 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.43336 -0.553705 -1.18372 1.86953 -0.584855 -1.79305 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.86953 -0.584855 -1.79305 2.16497 -0.744541 -2.46366 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.16497 -0.744541 -2.46366 2.47576 -1.04891 -3.07462 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.47576 -1.04891 -3.07462 2.83251 -1.17939 -3.7213 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.83251 -1.17939 -3.7213 3.05208 -1.07509 -4.43082 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0912362 0.127326 -0.0416614 0.620979 0.656403 -0.0858183 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.620979 0.656403 -0.0858183 1.063 1.26165 -0.113951 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.367522 -0.305015 0.0477331 -0.848008 -0.614659 -0.437811 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.848008 -0.614659 -0.437811 -1.19978 -1.07331 -0.915716 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.19978 -1.07331 -0.915716 -1.70486 -1.26538 -1.43581 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.70486 -1.26538 -1.43581 -2.28557 -1.64705 -1.71797 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.28557 -1.64705 -1.71797 -2.86239 -1.93218 -2.10329 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.86239 -1.93218 -2.10329 -3.4307 -2.30478 -2.42063 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.4307 -2.30478 -2.42063 -4.0094 -2.52618 -2.84322 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -4.0094 -2.52618 -2.84322 -4.67712 -2.49939 -3.1837 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.129592 0.277745 0.0779885 0.396531 0.834369 0.503916 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.396531 0.834369 0.503916 0.778178 1.39107 0.830915 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.778178 1.39107 0.830915 1.00663 1.8183 1.40344 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.00663 1.8183 1.40344 1.19808 2.1266 2.05979 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.19808 2.1266 2.05979 1.61613 2.16066 2.68155 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.61613 2.16066 2.68155 2.11715 2.34426 3.20859 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0677134 0.0225229 -0.250824 -0.06582 0.092177 -0.99758 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.06582 0.092177 -0.99758 0.215264 0.27007 -1.66978 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0333188 0.357751 0.139282 -0.167904 1.03235 0.43813 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.167904 1.03235 0.43813 -0.631152 1.62218 0.438593 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.631152 1.62218 0.438593 -0.882454 2.32138 0.540861 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.882454 2.32138 0.540861 -1.12101 3.01369 0.378703 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.161001 -0.358301 -0.0636788 0.57719 -0.724425 -0.568891 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.57719 -0.724425 -0.568891 0.906706 -0.9324 -1.20972 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.906706 -0.9324 -1.20972 1.41071 -0.86588 -1.76113 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.41071 -0.86588 -1.76113 1.69036 -0.723069 -2.44224 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.69036 -0.723069 -2.44224 1.92801 -0.284926 -3.00264 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.92801 -0.284926 -3.00264 2.15519 0.342783 -3.3445 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.15519 0.342783 -3.3445 2.53601 0.726425 -3.8644 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.53601 0.726425 -3.8644 3.08169 0.936101 -4.33426 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.08169 0.936101 -4.33426 3.45038 1.27772 -4.89092 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.45038 1.27772 -4.89092 3.53222 1.43383 -5.61991 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0753455 -0.198326 -0.218971 0.124503 -0.595449 -0.853303 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.124503 -0.595449 -0.853303 0.0694443 -1.02874 -1.463 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0694443 -1.02874 -1.463 0.204806 -1.69954 -1.76994 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.204806 -1.69954 -1.76994 0.130901 -2.10295 -2.39786 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.130901 -2.10295 -2.39786 -0.0540299 -2.36044 -3.07757 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0487631 -0.418862 0.181322 -0.061444 -0.982416 0.663776 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.061444 -0.982416 0.663776 -0.0380966 -1.71203 0.835871 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0380966 -1.71203 0.835871 0.158806 -2.38475 1.10267 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.158806 -2.38475 1.10267 0.241471 -3.08418 1.36048 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.241471 -3.08418 1.36048 0.284477 -3.77374 1.65229 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.284477 -3.77374 1.65229 0.597317 -4.39811 1.92577 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.597317 -4.39811 1.92577 1.15871 -4.77482 2.25047 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.15871 -4.77482 2.25047 1.84351 -5.07508 2.30866 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.236557 -0.231691 -0.307146 0.720843 -0.48455 -0.820985 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.153052 -0.376184 0.21644 -0.435739 -1.07085 0.211098 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.435739 -1.07085 0.211098 -0.679683 -1.75369 0.0194781 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.679683 -1.75369 0.0194781 -1.00733 -2.30073 -0.375367 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.00733 -2.30073 -0.375367 -1.45146 -2.84328 -0.641611 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.366191 -0.0207825 0.208147 1.05209 -0.144782 0.485046 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.05209 -0.144782 0.485046 1.75661 -0.0594969 0.727684 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.75661 -0.0594969 0.727684 2.43557 0.211723 0.894854 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.43557 0.211723 0.894854 3.10934 0.502266 1.05019 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.10934 0.502266 1.05019 3.69133 0.946686 1.21227 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.280663 -0.319218 -0.119054 -0.870363 -0.705042 -0.375751 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.870363 -0.705042 -0.375751 -1.26319 -1.15288 -0.831419 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.26319 -1.15288 -0.831419 -1.51859 -1.58991 -1.38484 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.51859 -1.58991 -1.38484 -1.50164 -2.11525 -1.91984 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.160812 -0.255788 0.190864 -0.21405 -1.0039 0.190566 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.21405 -1.0039 0.190566 -0.384677 -1.70755 -0.0050195 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.384677 -1.70755 -0.0050195 -0.822796 -2.27056 -0.236472 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.822796 -2.27056 -0.236472 -1.31429 -2.54897 -0.729851 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.31429 -2.54897 -0.729851 -1.64926 -2.91214 -1.29412 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.64926 -2.91214 -1.29412 -2.01479 -3.15578 -1.90201 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.142009 0.204357 0.02018 0.751546 0.64012 0.0529824 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.751546 0.64012 0.0529824 1.31639 1.05572 0.318937 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.31639 1.05572 0.318937 1.83504 1.45997 0.679604 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.83504 1.45997 0.679604 2.31869 1.69495 1.20245 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.31869 1.69495 1.20245 2.69345 1.64176 1.84993 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.414396 0.0596831 0.183365 -0.836266 -0.255946 0.717129 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.836266 -0.255946 0.717129 -1.21083 -0.524941 1.3086 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.21083 -0.524941 1.3086 -1.71146 -0.535662 1.86695 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.71146 -0.535662 1.86695 -2.098 -0.591372 2.50726 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.156659 -0.108828 -0.366369 0.0418774 -0.0484562 -1.10507 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0418774 -0.0484562 -1.10507 0.172831 -0.00324259 -1.84216 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.172831 -0.00324259 -1.84216 0.347238 0.200296 -2.54263 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0380247 0.240632 0.106171 0.178834 0.917695 0.396445 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.178834 0.917695 0.396445 0.114039 1.6019 0.696717 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.114039 1.6019 0.696717 0.318132 2.28845 0.919194 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.318132 2.28845 0.919194 0.716556 2.92186 0.969737 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.716556 2.92186 0.969737 0.826952 3.66288 1.00444 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.131634 0.0214976 -0.143866 0.365689 0.24394 -0.820798 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.365689 0.24394 -0.820798 0.736087 0.475059 -1.43063 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.736087 0.475059 -1.43063 0.854113 0.870553 -2.05685 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.854113 0.870553 -2.05685 1.10297 1.33875 -2.58728 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.291883 0.370767 -0.0194252 0.749316 0.942369 0.143442 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.749316 0.942369 0.143442 1.03722 1.6289 0.234446 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.03722 1.6289 0.234446 1.30828 2.2696 0.514684 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.30828 2.2696 0.514684 1.4273 3.00861 0.561545 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.4273 3.00861 0.561545 1.76744 3.6768 0.579638 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.115187 -0.204523 0.0577003 -0.406608 -0.813759 0.3839 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.406608 -0.813759 0.3839 -0.890989 -1.09889 0.880463 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.890989 -1.09889 0.880463 -1.4086 -1.3953 1.33513 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.166117 -0.0401561 0.0856268 -0.885616 -0.23161 0.176002 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.408414 -0.0811363 -0.00426136 -1.13817 -0.252448 0.0204144 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.13817 -0.252448 0.0204144 -1.88773 -0.22787 0.0277828 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.88773 -0.22787 0.0277828 -2.62634 -0.098336 0.0145746 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.62634 -0.098336 0.0145746 -3.32361 0.0211032 -0.234524 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.32361 0.0211032 -0.234524 -3.94636 -0.164861 -0.608829 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.94636 -0.164861 -0.608829 -4.36605 -0.164326 -1.23041 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.109858 0.181856 0.430538 0.248445 0.6254 1.01923 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.248445 0.6254 1.01923 0.344485 1.2915 1.35028 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.344485 1.2915 1.35028 0.393453 1.87187 1.8228 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.393453 1.87187 1.8228 0.462898 2.58169 2.05481 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.462898 2.58169 2.05481 0.501778 3.18455 2.49927 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0792053 0.267638 -0.0452445 0.14065 0.983827 -0.25925 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.14065 0.983827 -0.25925 0.394851 1.68709 -0.316679 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.394851 1.68709 -0.316679 0.691598 2.37008 -0.405945 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.691598 2.37008 -0.405945 1.01868 2.9826 -0.689378 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0714079 0.116593 0.0761388 -0.454769 0.74186 0.232904 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.454769 0.74186 0.232904 -0.81827 1.38891 0.341055 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.81827 1.38891 0.341055 -1.34262 1.9226 0.288828 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.34262 1.9226 0.288828 -1.83969 2.48419 0.295336 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.362983 0.105501 0.293575 0.979843 0.174769 0.714509 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.979843 0.174769 0.714509 1.6837 0.245626 0.963635 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.6837 0.245626 0.963635 2.40734 0.397609 1.08913 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.40734 0.397609 1.08913 3.08732 0.478104 1.39514 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.356232 -0.303513 -0.0675339 -1.02427 -0.626602 0.0412491 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.130131 -0.152051 -0.0889737 -0.501557 -0.681667 -0.468512 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.501557 -0.681667 -0.468512 -1.00521 -1.23312 -0.537354 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.00521 -1.23312 -0.537354 -1.57577 -1.71171 -0.448425 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.57577 -1.71171 -0.448425 -2.25982 -2.00447 -0.542557 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.25982 -2.00447 -0.542557 -2.74575 -2.48345 -0.853927 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.74575 -2.48345 -0.853927 -3.2494 -2.93909 -1.1721 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.2494 -2.93909 -1.1721 -3.72671 -3.46596 -1.41103 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.72671 -3.46596 -1.41103 -4.39004 -3.81528 -1.38955 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0715473 0.0127576 0.281855 -0.198891 -0.0114334 1.02057 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.198891 -0.0114334 1.02057 -0.156805 -0.00692111 1.76937 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.156805 -0.00692111 1.76937 -0.117982 0.0413643 2.51681 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.117982 0.0413643 2.51681 -0.289414 0.269342 3.21045 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.289414 0.269342 3.21045 -0.59897 0.444074 3.87086 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.59897 0.444074 3.87086 -0.892413 0.460576 4.56088 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0397644 -0.0678103 0.280127 0.522746 -0.499812 0.657755 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.522746 -0.499812 0.657755 0.995784 -1.0069 0.943416 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.995784 -1.0069 0.943416 1.48503 -1.40666 1.34755 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.48503 -1.40666 1.34755 1.77137 -2.03143 1.64784 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.77137 -2.03143 1.64784 1.81897 -2.75433 1.84188 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.271868 -0.0783296 -0.303692 -0.758181 0.0969238 -0.847094 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.758181 0.0969238 -0.847094 -1.22323 0.186118 -1.42871 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.22323 0.186118 -1.42871 -1.854 0.481558 -1.70681 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.854 0.481558 -1.70681 -2.43738 0.853178 -1.99675 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.43738 0.853178 -1.99675 -2.90432 1.27891 -2.40076 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.18806 0.088999 -0.296422 -0.461602 -0.0584106 -0.979024 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.461602 -0.0584106 -0.979024 -0.462002 -0.154942 -1.72279 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0309529 -0.285386 0.360388 -0.0388965 -0.699072 0.985928 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0388965 -0.699072 0.985928 -0.163452 -0.983309 1.66871 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0734913 -0.125756 0.373611 -0.246331 -0.288952 1.08494 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.246331 -0.288952 1.08494 -0.697355 -0.222193 1.68044 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.697355 -0.222193 1.68044 -0.915318 -0.0349045 2.3732 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.915318 -0.0349045 2.3732 -1.38911 -0.153801 2.94231 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.38911 -0.153801 2.94231 -1.84901 -0.254563 3.52612 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.84901 -0.254563 3.52612 -2.35477 -0.145664 4.06912 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.35477 -0.145664 4.06912 -2.98746 -0.0590506 4.46244 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.119632 -0.0450328 -0.0571596 0.771858 -0.407377 -0.133374 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.771858 -0.407377 -0.133374 1.18941 -1.02454 -0.048176 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.18941 -1.02454 -0.048176 1.33728 -1.75078 0.0667971 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.33728 -1.75078 0.0667971 1.25644 -2.42696 0.381024 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.185919 -0.385052 -0.00347579 0.67647 -0.863545 0.301324 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.67647 -0.863545 0.301324 1.03228 -1.52115 0.360172 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.03228 -1.52115 0.360172 1.34408 -2.17727 0.173668 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.34408 -2.17727 0.173668 1.54481 -2.89989 0.178118 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.54481 -2.89989 0.178118 1.74952 -3.60711 0.321054 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.12641 -0.0505601 -0.285573 0.574256 -0.292086 -0.836571 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.574256 -0.292086 -0.836571 0.73882 -0.494277 -1.5398 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.73882 -0.494277 -1.5398 0.891764 -0.821502 -2.1971 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.891764 -0.821502 -2.1971 0.779897 -1.36542 -2.70122 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.179946 0.0856924 -0.050444 0.794022 0.284362 -0.432467 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.371771 0.133791 0.293704 0.920141 0.282056 0.783405 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.920141 0.282056 0.783405 1.50653 0.38259 1.24006 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.50653 0.38259 1.24006 2.04228 0.361856 1.76451 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.196005 0.106 0.269878 -0.663801 0.555005 0.646789 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.663801 0.555005 0.646789 -1.17568 1.00152 0.964756 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.17568 1.00152 0.964756 -1.75988 1.42776 1.16358 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.203906 0.241868 -0.150481 0.737656 0.576043 -0.557835 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.737656 0.576043 -0.557835 1.19791 1.07608 -0.875052 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.19791 1.07608 -0.875052 1.42686 1.67529 -1.26369 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.00586883 0.433964 0.171476 -0.0634807 1.09782 0.513517 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0634807 1.09782 0.513517 0.00552401 1.65395 1.01197 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.00552401 1.65395 1.01197 0.161348 2.26865 1.41243 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.161348 2.26865 1.41243 0.0297523 2.95795 1.67709 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0297523 2.95795 1.67709 0.161592 3.41768 2.25482 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.161592 3.41768 2.25482 0.254415 3.66125 2.95806 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.456525 0.122302 -0.0852196 1.01242 0.548014 -0.354007 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.01242 0.548014 -0.354007 1.61026 0.994376 -0.430485 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.61026 0.994376 -0.430485 2.23601 1.20997 -0.783265 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.23601 1.20997 -0.783265 2.89929 1.53449 -0.914611 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0449853 -0.145451 -0.0490125 -0.145148 -0.83834 -0.318034 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.145148 -0.83834 -0.318034 -0.223331 -1.36313 -0.848115 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.223331 -1.36313 -0.848115 -0.51432 -1.61781 -1.49074 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.51432 -1.61781 -1.49074 -0.879645 -1.6287 -2.14566 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.00313802 -0.201247 -0.0924053 0.155838 -0.879271 -0.374301 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.155838 -0.879271 -0.374301 0.453337 -1.56762 -0.361411 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.453337 -1.56762 -0.361411 0.579499 -2.30158 -0.450254 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.579499 -2.30158 -0.450254 0.670186 -3.02684 -0.618411 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.110338 0.177388 -0.255474 0.699952 0.527202 -0.559588 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0673232 0.132298 0.168759 -0.454696 0.677817 0.507658 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.454696 0.677817 0.507658 -0.790079 1.34652 0.561024 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.790079 1.34652 0.561024 -1.12034 1.92245 0.909932 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.12034 1.92245 0.909932 -1.4473 2.31029 1.46236 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.4473 2.31029 1.46236 -1.9245 2.82388 1.72884 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.9245 2.82388 1.72884 -2.20758 3.48512 1.94126 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.205172 0.0304948 -0.29803 0.407915 -0.00887778 -1.01903 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.407915 -0.00887778 -1.01903 0.577973 0.0574339 -1.74648 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.577973 0.0574339 -1.74648 0.897814 0.360569 -2.35337 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.897814 0.360569 -2.35337 1.01045 0.683105 -3.02104 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.223296 -0.219072 -0.322648 0.630144 -0.663619 -0.769137 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.630144 -0.663619 -0.769137 1.0222 -0.87974 -1.37087 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.110731 0.0390073 0.145805 -0.503118 -0.108318 0.76776 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.503118 -0.108318 0.76776 -0.880978 -0.4801 1.29833 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.880978 -0.4801 1.29833 -1.08368 -0.579621 2.01352 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.08368 -0.579621 2.01352 -1.41392 -0.758746 2.66264 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.234434 0.0585441 -0.0685695 -0.840805 -0.153266 -0.455804 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.840805 -0.153266 -0.455804 -1.45101 -0.398591 -0.816306 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.45101 -0.398591 -0.816306 -1.971 -0.763314 -1.21517 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.971 -0.763314 -1.21517 -2.3607 -1.21253 -1.67216 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.3607 -1.21253 -1.67216 -2.91853 -1.6488 -1.91913 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.91853 -1.6488 -1.91913 -3.14734 -2.23172 -2.33186 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.14734 -2.23172 -2.33186 -3.14139 -2.84925 -2.75744 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.14139 -2.84925 -2.75744 -3.44271 -3.45252 -3.08572 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.44271 -3.45252 -3.08572 -3.74085 -4.00878 -3.49093 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0553417 0.0103792 -0.457059 0.310245 0.29852 -1.10088 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.310245 0.29852 -1.10088 0.404617 0.404536 -1.83732 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.404617 0.404536 -1.83732 0.737646 0.41853 -2.50918 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.737646 0.41853 -2.50918 0.831181 0.551188 -3.24141 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0351865 0.167559 -0.0954766 -0.475847 0.582078 -0.538751 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.475847 0.582078 -0.538751 -0.895228 0.797459 -1.12204 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.895228 0.797459 -1.12204 -1.0756 0.768731 -1.84946 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.0756 0.768731 -1.84946 -1.41967 0.742324 -2.51536 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.41967 0.742324 -2.51536 -1.55109 1.01176 -3.20285 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.55109 1.01176 -3.20285 -1.71341 1.38346 -3.83371 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.71341 1.38346 -3.83371 -1.91325 1.9392 -4.29601 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.91325 1.9392 -4.29601 -2.09002 2.56243 -4.67394 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.09002 2.56243 -4.67394 -2.14156 3.07113 -5.22264 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.12557 0.110476 0.409959 0.205991 0.535471 1.02267 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.273002 0.167329 -0.317378 -0.822614 0.535639 -0.670612 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.822614 0.535639 -0.670612 -1.48178 0.800893 -0.910702 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.48178 0.800893 -0.910702 -1.96879 1.36976 -0.95208 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.96879 1.36976 -0.95208 -2.4275 1.94791 -0.818584 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.4275 1.94791 -0.818584 -2.75648 2.61989 -0.870769 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.263063 -0.0331674 -0.062638 -1.01244 -0.0509704 -0.0378441 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.01244 -0.0509704 -0.0378441 -1.68661 -0.355016 -0.162562 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.68661 -0.355016 -0.162562 -2.35943 -0.387368 -0.49236 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.35943 -0.387368 -0.49236 -3.09126 -0.244849 -0.573673 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.09126 -0.244849 -0.573673 -3.69963 0.0247663 -0.919645 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.230194 -0.13245 0.274513 -0.696288 -0.377341 0.808634 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.696288 -0.377341 0.808634 -1.13499 -0.719471 1.31161 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.13499 -0.719471 1.31161 -1.36496 -1.08395 1.92542 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.36496 -1.08395 1.92542 -1.74368 -1.23846 2.55407 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.74368 -1.23846 2.55407 -1.86661 -1.27323 3.29311 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.86661 -1.27323 3.29311 -2.02903 -1.41114 4.01221 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.02903 -1.41114 4.01221 -1.94917 -1.44423 4.75721 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.281956 0.281482 -0.0607067 -0.90655 0.666787 0.0939604 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.90655 0.666787 0.0939604 -1.34511 1.22881 0.326964 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.34511 1.22881 0.326964 -1.92619 1.66931 0.502499 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.356472 -0.22951 0.0477179 -0.703432 -0.600832 0.599296 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.703432 -0.600832 0.599296 -1.22032 -1.03345 0.928181 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.22032 -1.03345 0.928181 -1.51481 -1.39878 1.51325 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.51481 -1.39878 1.51325 -1.63811 -1.85354 2.09677 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.63811 -1.85354 2.09677 -1.62089 -2.20322 2.76004 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.199094 -0.25531 0.0829028 -0.789643 -0.665253 0.296676 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.789643 -0.665253 0.296676 -1.46088 -0.952957 0.467443 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.141865 -0.0873983 -0.359891 0.376559 -0.189882 -1.06481 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.376559 -0.189882 -1.06481 0.770798 -0.265941 -1.69829 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.770798 -0.265941 -1.69829 1.16126 -0.199645 -2.33519 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.16126 -0.199645 -2.33519 1.47615 -0.162189 -3.01485 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.47615 -0.162189 -3.01485 1.90561 0.0820652 -3.57912 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0694183 -0.0860733 -0.243164 0.174374 -0.11737 -0.985124 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.174374 -0.11737 -0.985124 -0.00576387 -0.331741 -1.68089 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.00576387 -0.331741 -1.68089 -0.0905652 -0.299632 -2.42539 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0905652 -0.299632 -2.42539 -0.17192 -0.112877 -3.1472 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.144611 -0.343895 -0.0925417 0.282329 -1.07815 -0.026219 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0436413 0.342743 -0.259432 0.0696346 0.67963 -0.919868 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0696346 0.67963 -0.919868 0.198731 1.18822 -1.45575 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.198731 1.18822 -1.45575 0.0976934 1.76201 -1.92803 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0976934 1.76201 -1.92803 -0.134988 2.43865 -2.15281 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.134988 2.43865 -2.15281 -0.2542 3.15554 -2.33816 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.2542 3.15554 -2.33816 -0.47976 3.84958 -2.51119 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.47976 3.84958 -2.51119 -0.440049 4.46298 -2.94091 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.440049 4.46298 -2.94091 -0.324043 5.18155 -3.12172 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.202731 -0.252086 0.194851 0.692074 -0.78418 0.394648 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.692074 -0.78418 0.394648 1.16335 -1.36728 0.414547 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.16335 -1.36728 0.414547 1.62472 -1.95589 0.358195 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.62472 -1.95589 0.358195 2.01303 -2.54124 0.0953629 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.01303 -2.54124 0.0953629 2.60577 -2.92277 -0.16075 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.385282 0.264207 -0.110135 1.09547 0.253538 -0.351016 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0244648 0.436621 -0.0577735 0.0579709 1.10395 -0.398424 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0579709 1.10395 -0.398424 0.119523 1.79824 -0.675335 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.119523 1.79824 -0.675335 -7.97957e-06 2.4198 -1.07768 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -7.97957e-06 2.4198 -1.07768 -0.134749 3.09489 -1.37533 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.397501 0.265075 0.0400614 0.906494 0.814402 -0.000760686 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.906494 0.814402 -0.000760686 1.32354 1.39609 0.223324 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.32354 1.39609 0.223324 1.89713 1.80278 0.484262 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.89713 1.80278 0.484262 2.45761 2.17698 0.813424 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.205956 0.0439562 0.338719 0.569861 0.260303 0.957805 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.569861 0.260303 0.957805 0.745105 0.637288 1.58204 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.745105 0.637288 1.58204 1.10307 0.805527 2.21927 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.00905448 -0.257227 -0.128158 -0.45915 -0.846667 -0.239846 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.45915 -0.846667 -0.239846 -0.964246 -1.36973 -0.0560552 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.964246 -1.36973 -0.0560552 -1.21446 -2.03434 0.185164 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.21446 -2.03434 0.185164 -1.193 -2.69293 0.543358 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.193 -2.69293 0.543358 -1.14703 -3.27889 1.00924 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0566892 -0.118852 0.0665142 -0.665185 -0.496455 0.289334 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.665185 -0.496455 0.289334 -1.2044 -0.740697 0.749865 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.2044 -0.740697 0.749865 -1.67384 -0.78444 1.33314 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.67384 -0.78444 1.33314 -2.23583 -0.890223 1.81841 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.313196 0.335751 0.0300566 0.589671 1.01368 0.192783 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.589671 1.01368 0.192783 0.945683 1.66042 0.324981 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.945683 1.66042 0.324981 0.996095 2.3259 0.667179 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.996095 2.3259 0.667179 1.38495 2.95563 0.788545 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0089566 -0.245265 -0.23609 -0.0929874 -0.825225 -0.704155 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0929874 -0.825225 -0.704155 -0.0422251 -1.54146 -0.920803 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0422251 -1.54146 -0.920803 0.413432 -2.1325 -0.99527 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.413432 -2.1325 -0.99527 0.910645 -2.67797 -0.862071 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.910645 -2.67797 -0.862071 1.41315 -3.14941 -0.565875 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.41315 -3.14941 -0.565875 1.69849 -3.64481 -0.0804298 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.259279 -0.0820214 -0.0431814 0.896835 -0.476354 -0.0660841 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.896835 -0.476354 -0.0660841 1.55301 -0.767085 0.15166 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.55301 -0.767085 0.15166 2.29261 -0.889781 0.172643 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.29261 -0.889781 0.172643 3.02752 -1.03401 0.132609 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.296407 -0.11423 -0.354189 0.941319 -0.343329 -0.660951 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.941319 -0.343329 -0.660951 1.52914 -0.607286 -1.04474 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.52914 -0.607286 -1.04474 2.21214 -0.702495 -1.33959 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.21214 -0.702495 -1.33959 2.88332 -0.645847 -1.66946 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.335926 -0.139938 0.155927 -0.98945 -0.0846011 0.519718 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.98945 -0.0846011 0.519718 -1.67346 0.0628538 0.789689 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.67346 0.0628538 0.789689 -2.33106 0.141361 1.14169 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.33106 0.141361 1.14169 -2.97926 0.502712 1.25015 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.97926 0.502712 1.25015 -3.57544 0.830333 1.56596 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.57544 0.830333 1.56596 -3.94057 1.23024 2.08485 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.166357 -0.109532 0.427286 -0.122561 -0.279373 1.15649 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.122561 -0.279373 1.15649 -0.044641 -0.347901 1.89927 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.044641 -0.347901 1.89927 -0.0750325 -0.43867 2.64314 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0750325 -0.43867 2.64314 -0.400058 -0.798983 3.21501 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.250974 0.146332 0.363117 -0.378476 0.702047 0.850378 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.378476 0.702047 0.850378 -0.464806 1.30767 1.28428 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0688456 0.0932094 0.476517 0.51085 0.221208 1.06876 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.51085 0.221208 1.06876 0.692581 0.192897 1.79586 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.692581 0.192897 1.79586 1.0286 0.197533 2.46635 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.0286 0.197533 2.46635 1.10893 0.40614 3.18227 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0131456 0.208351 -0.239069 0.0829197 0.75459 -0.748239 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0829197 0.75459 -0.748239 0.402416 1.15409 -1.29671 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.402416 1.15409 -1.29671 0.482134 1.68892 -1.81642 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.482134 1.68892 -1.81642 0.921959 2.08094 -2.28051 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.921959 2.08094 -2.28051 1.44592 2.24325 -2.79199 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.102393 0.158323 -0.145222 0.38751 0.34875 -0.812265 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.38751 0.34875 -0.812265 0.935859 0.491988 -1.30348 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.935859 0.491988 -1.30348 1.42563 0.95959 -1.62594 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.42563 0.95959 -1.62594 1.95236 1.18154 -2.11151 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.95236 1.18154 -2.11151 2.50984 1.51626 -2.48526 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.50984 1.51626 -2.48526 3.1464 1.66469 -2.85303 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.1464 1.66469 -2.85303 3.88627 1.75942 -2.93129 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.88627 1.75942 -2.93129 4.59357 1.90646 -2.7298 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.2453 -0.184868 0.139895 -0.528866 -0.711688 0.592167 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.528866 -0.711688 0.592167 -0.534785 -1.43083 0.805019 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.534785 -1.43083 0.805019 -0.582448 -2.17102 0.916138 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.582448 -2.17102 0.916138 -0.442989 -2.90697 0.878463 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.442989 -2.90697 0.878463 -0.520114 -3.62006 1.09769 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.124919 -0.201716 -0.0836322 0.171215 -0.891924 -0.373407 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.171215 -0.891924 -0.373407 0.28827 -1.58649 -0.63104 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.28827 -1.58649 -0.63104 0.244173 -2.29965 -0.858989 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.244173 -2.29965 -0.858989 -0.0269757 -2.99791 -0.821483 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0269757 -2.99791 -0.821483 -0.377651 -3.62681 -1.03127 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.239909 0.270122 -0.178513 -0.737971 0.830471 -0.199568 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.737971 0.830471 -0.199568 -0.962618 1.53393 -0.330636 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0778426 -0.0680051 -0.233383 0.040944 -0.291882 -0.948238 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.040944 -0.291882 -0.948238 -0.0508507 -0.769865 -1.51886 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0508507 -0.769865 -1.51886 -0.0159781 -1.1614 -2.15759 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0159781 -1.1614 -2.15759 -0.0734398 -1.57779 -2.77874 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0734398 -1.57779 -2.77874 -0.160334 -2.10569 -3.30435 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.160334 -2.10569 -3.30435 -0.233011 -2.66827 -3.79498 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.265723 -0.108398 0.381332 -0.30702 -0.077405 1.12955 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.30702 -0.077405 1.12955 -0.0925955 0.20141 1.79196 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0925955 0.20141 1.79196 0.0807934 0.403344 2.49314 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0807934 0.403344 2.49314 -0.0589796 0.309108 3.22395 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.1198 0.268329 -0.214577 -0.210947 0.786799 -0.748787 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.210947 0.786799 -0.748787 -0.482898 1.37178 -1.13134 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.482898 1.37178 -1.13134 -1.04067 1.83198 -1.33034 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.04067 1.83198 -1.33034 -1.75793 2.03501 -1.24782 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.75793 2.03501 -1.24782 -2.28974 2.539 -1.08758 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.28974 2.539 -1.08758 -2.89272 2.98268 -1.0422 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.212087 -0.0676724 0.146284 0.770795 -0.246561 0.613557 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.302065 -0.0287394 0.258428 -0.950163 -0.402093 0.313899 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.950163 -0.402093 0.313899 -1.62672 -0.625809 0.547832 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.62672 -0.625809 0.547832 -2.31239 -0.703155 0.841727 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.31239 -0.703155 0.841727 -3.04329 -0.537533 0.870861 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.04329 -0.537533 0.870861 -3.76096 -0.556306 1.08788 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.76096 -0.556306 1.08788 -4.46535 -0.775086 1.22379 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -4.46535 -0.775086 1.22379 -5.14849 -1.08359 1.24924 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.207146 0.137233 0.0462393 -0.677222 0.567248 0.441986 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.677222 0.567248 0.441986 -1.00624 0.895773 1.03048 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.137073 -0.210715 0.123319 0.329374 -0.595052 0.737977 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.329374 -0.595052 0.737977 0.676397 -0.979861 1.28019 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.359946 0.13649 -0.197362 0.937648 0.370221 -0.614651 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.937648 0.370221 -0.614651 1.48809 0.652061 -1.03901 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0159653 0.0170365 0.205519 0.202818 -0.0646407 0.927263 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.384219 0.0601518 0.128402 1.12757 0.155654 0.156878 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.12757 0.155654 0.156878 1.85676 0.325959 0.199042 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.85676 0.325959 0.199042 2.49444 0.685255 0.0354266 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.49444 0.685255 0.0354266 3.19646 0.777848 -0.211749 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.19646 0.777848 -0.211749 3.94558 0.802566 -0.185092 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.127607 -0.370592 0.0361317 -0.186882 -1.01688 0.412021 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.186882 -1.01688 0.412021 -0.293914 -1.52521 0.952992 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.293914 -1.52521 0.952992 -0.36064 -2.18047 1.31172 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.36064 -2.18047 1.31172 -0.744798 -2.70214 1.68958 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.11975 0.294741 -0.244245 0.441315 0.742816 -0.752502 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.441315 0.742816 -0.752502 0.591541 0.963062 -1.45352 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.591541 0.963062 -1.45352 0.685998 1.40966 -2.0486 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.685998 1.40966 -2.0486 1.1146 1.78889 -2.53335 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0920325 0.149337 -0.168779 0.0860249 0.471341 -0.84611 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0860249 0.471341 -0.84611 0.255583 0.800459 -1.49836 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.255583 0.800459 -1.49836 0.317721 1.26917 -2.08056 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.317721 1.26917 -2.08056 0.617313 1.7147 -2.60424 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.617313 1.7147 -2.60424 0.957472 2.12889 -3.12888 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.957472 2.12889 -3.12888 1.47184 2.61495 -3.3772 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.47184 2.61495 -3.3772 1.96411 3.17442 -3.46188 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.96411 3.17442 -3.46188 2.14255 3.88891 -3.60388 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.14255 3.88891 -3.60388 2.44875 4.54972 -3.78296 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.00770957 0.353253 -0.0466383 -0.0368649 1.10215 -0.0183063 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0368649 1.10215 -0.0183063 0.0461569 1.83868 -0.132911 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0461569 1.83868 -0.132911 -0.0308399 2.58198 -0.0690604 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0308399 2.58198 -0.0690604 -0.415411 3.21756 0.0341035 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.415411 3.21756 0.0341035 -0.906073 3.69921 0.333714 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.906073 3.69921 0.333714 -1.48189 4.17867 0.366244 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.077163 0.107375 0.320291 -0.0282686 0.252452 1.0545 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0282686 0.252452 1.0545 -0.0152294 0.10114 1.78896 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0152294 0.10114 1.78896 -0.114504 0.0724875 2.53181 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.114504 0.0724875 2.53181 -0.438036 0.336893 3.15464 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.00370515 0.398411 -0.206022 -0.260588 0.945246 -0.650412 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.260588 0.945246 -0.650412 -0.52324 1.61833 -0.851582 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.52324 1.61833 -0.851582 -0.893065 2.19422 -1.15832 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.893065 2.19422 -1.15832 -1.51838 2.51248 -1.42325 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.194717 0.450971 -0.0158471 -0.587612 1.08982 -0.0135624 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.587612 1.08982 -0.0135624 -1.03044 1.67271 0.149687 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.03044 1.67271 0.149687 -1.48187 2.22764 0.374989 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.48187 2.22764 0.374989 -1.91125 2.80051 0.598486 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0844153 -0.468773 -0.0556598 0.272512 -1.14431 0.210362 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.272512 -1.14431 0.210362 0.535618 -1.84617 0.236224 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.535618 -1.84617 0.236224 0.52188 -2.59017 0.3299 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.52188 -2.59017 0.3299 0.79739 -3.28013 0.432631 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.79739 -3.28013 0.432631 1.19089 -3.87936 0.212222 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.331677 -0.0510217 -0.153266 0.952854 -0.161763 -0.558699 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.952854 -0.161763 -0.558699 1.56244 -0.398098 -0.926193 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.56244 -0.398098 -0.926193 2.01441 -0.96198 -1.12684 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.01441 -0.96198 -1.12684 2.54816 -1.44317 -1.34149 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.54816 -1.44317 -1.34149 2.91958 -2.04568 -1.58952 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.91958 -2.04568 -1.58952 3.17806 -2.64194 -1.9639 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0655612 -0.270424 -0.0283158 -0.363798 -0.912943 -0.27474 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.363798 -0.912943 -0.27474 -0.75606 -1.46408 -0.598592 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.75606 -1.46408 -0.598592 -1.16715 -1.99278 -0.936204 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.322764 0.177379 0.126568 -1.05053 0.345331 0.058347 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.05053 0.345331 0.058347 -1.74127 0.403602 -0.227973 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.74127 0.403602 -0.227973 -2.26621 0.722367 -0.658469 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.26621 0.722367 -0.658469 -2.79546 0.887028 -1.16373 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.342593 0.0222381 0.0445395 1.07765 0.0542007 -0.10095 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.07765 0.0542007 -0.10095 1.79391 0.269935 -0.0467664 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.79391 0.269935 -0.0467664 2.2826 0.801244 -0.250205 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.2826 0.801244 -0.250205 2.51196 1.47358 -0.490748 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.51196 1.47358 -0.490748 2.56447 2.17663 -0.746604 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.56447 2.17663 -0.746604 2.8397 2.80654 -1.04654 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.8397 2.80654 -1.04654 2.87512 3.55338 -1.10543 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.35264 0.24747 -0.167052 0.994802 0.557901 -0.398915 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.994802 0.557901 -0.398915 1.569 0.932416 -0.703107 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.569 0.932416 -0.703107 2.03493 1.18994 -1.23139 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.03493 1.18994 -1.23139 2.35773 1.13458 -1.90611 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.35773 1.13458 -1.90611 2.6776 1.12179 -2.58435 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.6776 1.12179 -2.58435 3.24583 1.14483 -3.07332 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0575497 0.0473867 -0.295869 0.320154 -0.0217774 -0.99498 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.320154 -0.0217774 -0.99498 0.760685 0.172376 -1.57008 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.760685 0.172376 -1.57008 1.24277 0.523763 -2.02463 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.24277 0.523763 -2.02463 1.66011 0.809613 -2.57836 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.66011 0.809613 -2.57836 2.08534 1.05624 -3.14481 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.08534 1.05624 -3.14481 2.59822 1.14721 -3.68441 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.59822 1.14721 -3.68441 3.21277 1.26537 -4.09778 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.21277 1.26537 -4.09778 3.86632 1.52782 -4.35562 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.86632 1.52782 -4.35562 4.35512 2.0964 -4.33823 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 4.35512 2.0964 -4.33823 4.69741 2.65319 -3.97037 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.231459 -0.22897 0.0125832 -0.848409 -0.574355 0.262745 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.102558 -0.0900184 -0.10815 -0.213319 -0.604562 -0.64245 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.213319 -0.604562 -0.64245 -0.550314 -0.97822 -1.19861 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.550314 -0.97822 -1.19861 -0.751333 -1.29369 -1.84866 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.751333 -1.29369 -1.84866 -1.21082 -1.48031 -2.41128 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.111177 -0.201893 -0.0442753 -0.63469 -0.73641 -0.0964772 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.63469 -0.73641 -0.0964772 -1.00743 -1.33317 -0.35619 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0766093 0.48055 -0.00865811 0.181752 1.1597 -0.194419 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.181752 1.1597 -0.194419 0.27587 1.89903 -0.11059 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.27587 1.89903 -0.11059 0.288721 2.64592 -0.177596 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.288721 2.64592 -0.177596 0.504012 3.271 -0.53175 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.504012 3.271 -0.53175 0.829434 3.93174 -0.673275 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.256451 -0.0994267 -0.247864 -0.754515 -0.228739 -0.793492 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.754515 -0.228739 -0.793492 -1.40193 -0.314165 -1.16235 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.112621 -0.245576 -0.385635 0.366357 -0.357275 -1.08251 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.366357 -0.357275 -1.08251 0.715572 -0.281177 -1.74188 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.715572 -0.281177 -1.74188 0.917926 -0.182622 -2.45731 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.917926 -0.182622 -2.45731 1.01546 -0.108117 -3.19719 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.120365 -0.384607 0.0850561 0.650257 -0.915145 0.0694503 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.650257 -0.915145 0.0694503 1.1809 -1.35664 -0.223809 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.1809 -1.35664 -0.223809 1.72138 -1.80679 -0.484097 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.72138 -1.80679 -0.484097 2.20668 -2.37858 -0.489998 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.389435 -0.136221 0.267808 -0.950003 -0.39716 0.692277 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.950003 -0.39716 0.692277 -1.6327 -0.570393 0.94998 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.6327 -0.570393 0.94998 -2.36077 -0.515823 1.12154 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.291846 -0.0829853 0.15618 -0.902636 -0.23011 0.565802 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.902636 -0.23011 0.565802 -1.35048 -0.507996 1.09939 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.35048 -0.507996 1.09939 -1.93192 -0.633443 1.55622 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.93192 -0.633443 1.55622 -2.54408 -0.821766 1.94646 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.54408 -0.821766 1.94646 -3.21259 -0.911429 2.27441 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.21259 -0.911429 2.27441 -3.83346 -1.20223 2.57849 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.213751 0.202665 0.317918 0.72707 0.31362 0.853355 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.72707 0.31362 0.853355 1.40355 0.488708 1.12578 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.40355 0.488708 1.12578 2.07906 0.440907 1.44813 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.07906 0.440907 1.44813 2.73144 0.307205 1.79312 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.73144 0.307205 1.79312 3.46348 0.249322 1.94565 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.46348 0.249322 1.94565 4.07148 0.454653 2.33382 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 4.07148 0.454653 2.33382 4.64071 0.763111 2.71241 0.0375
XformPop
XformPush
Color 1 1 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 4.64071 0.763111 2.71241 5.20559 1.2377 2.84725 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.368776 -0.296011 -0.0534418 0.67955 -0.924319 -0.32018 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.67955 -0.924319 -0.32018 1.11974 -1.53152 -0.326983 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.11974 -1.53152 -0.326983 1.66318 -2.04468 -0.265093 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.66318 -2.04468 -0.265093 1.96107 -2.6874 -0.0187718 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.96107 -2.6874 -0.0187718 2.02209 -3.42163 0.12156 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.02209 -3.42163 0.12156 2.26878 -3.98487 0.550969 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.26878 -3.98487 0.550969 2.46351 -4.61759 0.903468 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0152366 -0.179455 -0.157764 -0.0393779 -0.623644 -0.759605 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0393779 -0.623644 -0.759605 -0.338514 -1.14514 -1.20801 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.338514 -1.14514 -1.20801 -0.756639 -1.55754 -1.67448 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.756639 -1.55754 -1.67448 -0.875131 -2.1412 -2.13033 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.01543 0.243943 0.224831 -0.246246 0.879838 0.524269 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.246246 0.879838 0.524269 -0.307561 1.47308 0.979029 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.307561 1.47308 0.979029 -0.501897 1.91973 1.54932 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.501897 1.91973 1.54932 -0.758556 2.14624 2.21664 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.758556 2.14624 2.21664 -0.839977 2.44694 2.89888 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0716116 -0.114105 -0.0914081 -0.0141361 -0.461448 -0.753638 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0141361 -0.461448 -0.753638 -0.175142 -0.568059 -1.47835 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.175142 -0.568059 -1.47835 -0.464876 -0.76359 -2.14192 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.464876 -0.76359 -2.14192 -0.881987 -0.837859 -2.76079 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.881987 -0.837859 -2.76079 -1.49363 -0.94962 -3.1802 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0851016 0.350647 0.036985 -0.371745 1.04362 0.0261537 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.371745 1.04362 0.0261537 -0.694768 1.6845 -0.191648 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.694768 1.6845 -0.191648 -1.07283 2.33017 -0.139836 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.07283 2.33017 -0.139836 -1.37952 3.00529 -0.252263 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.37952 3.00529 -0.252263 -1.80665 3.55454 -0.532241 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.185886 -0.304802 0.255501 0.484157 -0.972989 0.42 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.484157 -0.972989 0.42 0.512142 -1.72066 0.367938 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.512142 -1.72066 0.367938 0.653775 -2.41497 0.122229 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.653775 -2.41497 0.122229 0.748919 -2.98191 -0.359459 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.748919 -2.98191 -0.359459 1.14899 -3.53952 -0.661982 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.294358 0.190273 -0.182783 -0.945846 0.375278 -0.505019 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.945846 0.375278 -0.505019 -1.52799 0.379072 -0.977877 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.52799 0.379072 -0.977877 -2.16028 0.484753 -1.36716 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.16028 0.484753 -1.36716 -2.75859 0.302462 -1.78104 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.75859 0.302462 -1.78104 -3.26405 0.0118403 -2.25279 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.26405 0.0118403 -2.25279 -3.6218 -0.092073 -2.90373 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.31405 -0.123979 0.275422 -0.882595 -0.55074 0.514442 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.882595 -0.55074 0.514442 -1.58656 -0.809115 0.527584 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.58656 -0.809115 0.527584 -2.32513 -0.90747 0.613324 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.32513 -0.90747 0.613324 -3.06794 -1.0104 0.62474 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.185227 0.135038 0.380903 -0.477434 0.581957 0.90757 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.477434 0.581957 0.90757 -0.946262 1.10234 1.17572 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.946262 1.10234 1.17572 -1.33767 1.74034 1.22318 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.33767 1.74034 1.22318 -1.63511 2.39735 1.42902 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.078071 0.223457 0.0623102 0.490231 0.844997 -0.017137 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.490231 0.844997 -0.017137 0.999691 1.2959 -0.332798 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.999691 1.2959 -0.332798 1.36247 1.95055 -0.380977 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.36247 1.95055 -0.380977 1.78596 2.56337 -0.293795 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.78596 2.56337 -0.293795 1.93365 3.28543 -0.432813 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.102015 -0.399458 0.114126 0.0531781 -1.03621 0.478759 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0531781 -1.03621 0.478759 0.266502 -1.73804 0.635068 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.266502 -1.73804 0.635068 0.405643 -2.47058 0.55431 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.405643 -2.47058 0.55431 0.757516 -3.05204 0.237152 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.757516 -3.05204 0.237152 1.13365 -3.52656 -0.205405 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.260264 0.227518 0.282358 -0.381477 0.879611 0.632475 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.381477 0.879611 0.632475 -0.370187 1.49679 1.05846 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.190269 0.153783 -0.0803911 -0.903696 0.307903 -0.252928 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.903696 0.307903 -0.252928 -1.56546 0.31353 -0.605833 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.56546 0.31353 -0.605833 -2.12602 0.109614 -1.06046 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.12602 0.109614 -1.06046 -2.79283 0.212661 -1.38795 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.79283 0.212661 -1.38795 -3.38794 0.0360672 -1.80884 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.148342 0.0344597 -0.213851 0.482347 0.152847 -0.874855 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.263592 -0.115533 -0.236091 -0.829987 -0.293894 -0.694223 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.829987 -0.293894 -0.694223 -1.35568 -0.510925 -1.18315 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.35568 -0.510925 -1.18315 -1.88701 -0.788593 -1.6338 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.88701 -0.788593 -1.6338 -2.4153 -0.772043 -2.1659 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.405434 0.148662 0.236932 -1.09889 -0.113654 0.350108 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.09889 -0.113654 0.350108 -1.78228 -0.422031 0.330428 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.78228 -0.422031 0.330428 -2.52577 -0.519109 0.313403 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.52577 -0.519109 0.313403 -3.2618 -0.663168 0.309684 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.172097 -0.110377 -0.0856414 0.432907 -0.580651 -0.608442 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.208871 -0.433574 0.0515121 -0.389143 -1.15204 0.169006 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.389143 -1.15204 0.169006 -0.639587 -1.85377 0.0832487 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.639587 -1.85377 0.0832487 -1.03849 -2.48629 0.14071 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.03849 -2.48629 0.14071 -1.48743 -3.03585 -0.102046 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.48743 -3.03585 -0.102046 -2.02965 -3.35963 -0.506595 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.02965 -3.35963 -0.506595 -2.75236 -3.50924 -0.640064 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.247772 -0.093982 0.353863 -0.578646 0.0252282 1.01629 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.578646 0.0252282 1.01629 -0.732262 0.169092 1.73616 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.12648 0.00744909 0.243098 0.313045 0.176009 0.949697 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.313045 0.176009 0.949697 0.496908 0.512831 1.59409 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.496908 0.512831 1.59409 0.8212 1.01352 2.04867 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.8212 1.01352 2.04867 1.28525 1.23047 2.59648 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.376291 -0.289785 -0.0487317 0.835344 -0.878482 0.0234251 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.465527 0.0147722 -0.00125327 -1.1753 -0.166405 0.159672 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.1753 -0.166405 0.159672 -1.87553 -0.360104 0.345854 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.87553 -0.360104 0.345854 -2.60037 -0.54943 0.381424 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.60037 -0.54943 0.381424 -3.32805 -0.658068 0.526922 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -3.32805 -0.658068 0.526922 -4.01189 -0.581114 0.825151 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -4.01189 -0.581114 0.825151 -4.68386 -0.310857 1.01988 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.190817 0.235324 -0.377556 0.660143 0.708378 -0.721726 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.660143 0.708378 -0.721726 1.30771 1.02529 -0.928431 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.30771 1.02529 -0.928431 1.94658 1.37636 -1.10475 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.94658 1.37636 -1.10475 2.35592 1.95668 -1.34595 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.35592 1.95668 -1.34595 2.61852 2.49637 -1.7957 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.61852 2.49637 -1.7957 2.96057 3.13561 -1.98771 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0622107 0.042204 0.012316 0.701977 0.426133 -0.0638228 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.701977 0.426133 -0.0638228 1.26433 0.857738 -0.308715 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.26433 0.857738 -0.308715 1.86138 1.3115 -0.297032 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.86138 1.3115 -0.297032 2.37266 1.82707 -0.109205 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.37266 1.82707 -0.109205 2.97915 2.26191 -0.0344717 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.97915 2.26191 -0.0344717 3.67046 2.49803 -0.204278 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.326095 -0.165452 -0.248249 0.774053 -0.55584 -0.705885 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.774053 -0.55584 -0.705885 1.04996 -1.15036 -1.07047 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.04996 -1.15036 -1.07047 1.42748 -1.79632 -1.12263 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.42748 -1.79632 -1.12263 2.00632 -2.27256 -1.14781 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.00632 -2.27256 -1.14781 2.57786 -2.72801 -1.31635 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.17601 0.27796 0.0999108 0.361256 1.00313 0.0517749 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.361256 1.00313 0.0517749 0.492242 1.7179 -0.133796 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.216692 -0.43821 -0.104739 0.864137 -0.771689 0.0744457 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.864137 -0.771689 0.0744457 1.31211 -1.26866 0.413313 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.31211 -1.26866 0.413313 1.55388 -1.85664 0.81123 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.55388 -1.85664 0.81123 1.94302 -2.42373 1.11036 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.393313 -0.0761602 0.0389008 1.06565 -0.296477 0.287747 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.06565 -0.296477 0.287747 1.6564 -0.306589 0.749712 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.6564 -0.306589 0.749712 2.17874 -0.194886 1.27619 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.17874 -0.194886 1.27619 2.42931 -0.0344415 1.96465 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.258282 0.0226531 -0.202765 0.844441 0.147065 -0.653807 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.0586501 0.230743 -0.0956768 -0.24659 0.955129 -0.145109 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.24659 0.955129 -0.145109 -0.295407 1.68631 0.0145366 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.295407 1.68631 0.0145366 -0.562311 2.38433 -0.048964 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.562311 2.38433 -0.048964 -0.957311 2.99897 -0.218344 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.957311 2.99897 -0.218344 -1.42259 3.5872 -0.215485 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.42259 3.5872 -0.215485 -1.72994 4.2675 -0.287753 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.34528 0.0637271 0.0883839 -0.923372 0.0906039 0.565445 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -0.923372 0.0906039 0.565445 -1.58205 0.371966 0.787888 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -1.58205 0.371966 0.787888 -2.21753 0.761224 0.872435 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.21753 0.761224 0.872435 -2.89859 1.07412 0.845197 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube -2.89859 1.07412 0.845197 -3.44648 1.58454 0.802899 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.0754457 -0.076258 0.0981451 0.70014 -0.315993 0.436944 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 0.70014 -0.315993 0.436944 1.2202 -0.688109 0.828823 0.0375
XformPop
XformPush
Color 1 0.3 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.2202 -0.688109 0.828823 1.82119 -1.02598 1.12404 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 1.82119 -1.02598 1.12404 2.44338 -1.44256 1.1669 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 2.44338 -1.44256 1.1669 3.16226 -1.63584 1.07553 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.16226 -1.63584 1.07553 3.87178 -1.79681 0.89339 0.0375
XformPop
XformPush
Color 0.3 0.7 1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
Tube 3.87178 -1.79681 0.89339 4.62129 -1.81721 0.875539 0.0375
XformPop
WorldEnd
