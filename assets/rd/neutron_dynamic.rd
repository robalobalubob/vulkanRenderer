# Neutron Diffusion Simulation - Frame 1
Display "Neutron Diffusion" "Screen" "rgbdouble"
Format 800 600
CameraEye 0 5 20
CameraAt 0 0 0
CameraUp 0 1 0
CameraFOV 30
Background 0.05 0.05 0.15

FrameBegin 1
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 85 neutrons
Color 1 0.2 0
XformPush
Translate -2.33286 -1.07753 -1.86086
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.52213 -0.84219 0.994439
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.514127 0.199672 -2.69333
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.798965 -2.51895 -1.32871
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.93437 1.35263 -1.40984
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.58729 -0.750597 -1.72447
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.04469 2.63431 -2.05455
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.897017 -2.3231 -2.20958
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.09799 3.04131 0.42592
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.72691 -0.553955 -1.91723
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.379223 -0.599202 2.9473
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.0503857 2.93949 1.01722
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.922484 1.71119 2.33767
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.32794 2.73241 0.189805
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.03078 0.277313 0.485281
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.100498 -2.93034 -0.82424
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.03032 1.61984 1.8209
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.722716 -2.37912 -1.87868
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.721396 -0.244718 2.66206
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.872927 1.71137 2.86582
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.322544 -0.697505 3.40762
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.503356 1.60962 2.36293
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.413829 0.258444 -3.27449
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.929782 2.84621 0.515796
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.99447 1.7045 2.28319
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.10566 2.67809 1.02736
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.51959 2.95921 0.847587
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.06279 2.43895 0.565247
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.8212 -1.85939 -1.33707
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.221785 1.09248 2.73786
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.31724 -0.303232 3.25457
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.36435 -2.24649 -1.38536
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.70564 1.23583 -1.50823
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.77785 -0.863402 2.50166
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.821191 2.28181 -1.86034
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.04083 -0.403069 -0.849222
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.61912 0.92996 0.522578
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.31416 -2.4004 1.54592
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.38717 -1.0461 -0.308869
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.068936 -0.0314335 0.24339
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.37312 0.642375 -2.28135
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.46517 1.77635 0.174619
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.47779 1.57601 2.1888
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.199363 2.33004 -1.36698
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.1675 2.03539 -1.61592
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.359367 -3.30772 -0.465858
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.19616 0.891762 -2.3985
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.30942 1.95515 0.619168
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.03383 -1.37483 -1.57467
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.774999 -1.87455 2.45997
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.406773 -3.24602 -0.136709
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.94105 0.468792 1.89654
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.60993 2.32686 2.06528
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.88308 -2.77825 -0.503411
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.97732 2.84245 0.747804
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.62394 1.79453 -0.0553177
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.09001 2.28763 -0.831619
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.919862 2.98341 -0.55513
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.18567 1.66841 -0.634584
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.80366 -0.362683 1.53302
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.90365 1.7584 1.6674
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.25164 1.79134 -0.129524
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.674866 -2.55905 -0.870725
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.970531 -2.86077 0.375911
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.90429 -0.121718 -0.976339
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.58643 -0.862087 2.1681
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.363119 -2.29111 -1.41339
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.02195 -0.934803 1.57525
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.09767 -2.67513 -1.90015
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.43541 -1.87336 -1.55703
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.813038 2.66299 1.12729
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.88765 -1.08108 0.239749
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.00843 -0.317479 -0.115826
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.867151 -1.95276 2.17671
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.959226 0.263423 3.51499
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.48985 -2.4547 1.66129
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.33845 -0.803034 -2.48129
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.535058 1.34023 -2.54116
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.03344 -2.75141 0.688399
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.525741 0.79586 3.41979
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.07826 -1.53924 2.23827
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 3 neutrons
Color 1 0.8 0
XformPush
Translate -1.14807 -1.86791 -0.552302
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 1.23442 1.62859 0.947964
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 2.14434 -0.898186 -0.413925
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
LineSet "PC"
60 30
  -2.33286 -1.07753 -1.86086 1 0.2 0
  -3.49291 -1.70361 -2.73395 1 0.2 0
  -2.52213 -0.84219 0.994439 1 0.2 0
  -3.83549 -1.33552 1.72363 1 0.2 0
  -0.514127 0.199672 -2.69333 1 0.2 0
  -0.782401 0.183457 -4.25146 1 0.2 0
  -0.798965 -2.51895 -1.32871 1 0.2 0
  -1.34877 -3.74083 -2.16819 1 0.2 0
  -1.93437 1.35263 -1.40984 1 0.2 0
  -3.07442 2.08624 -2.22354 1 0.2 0
  -2.58729 -0.750597 -1.72447 1 0.2 0
  -3.80274 -0.966185 -2.7125 1 0.2 0
  1.04469 2.63431 -2.05455 1 0.2 0
  1.50679 3.74172 -3.08417 1 0.2 0
  0.897017 -2.3231 -2.20958 1 0.2 0
  1.44191 -3.43585 -3.19186 1 0.2 0
  -1.09799 3.04131 0.42592 1 0.2 0
  -1.74212 4.43763 0.793844 1 0.2 0
  2.72691 -0.553955 -1.91723 1 0.2 0
  4.0259 -0.684055 -2.80926 1 0.2 0
  -0.379223 -0.599202 2.9473 1 0.2 0
  -0.410235 -0.944566 4.48995 1 0.2 0
  0.0503857 2.93949 1.01722 1 0.2 0
  0.192987 4.42744 1.53264 1 0.2 0
  -0.922484 1.71119 2.33767 1 0.2 0
  -1.38471 2.44821 3.65795 1 0.2 0
  -1.32794 2.73241 0.189805 1 0.2 0
  -1.96558 4.17902 0.217196 1 0.2 0
  3.03078 0.277313 0.485281 1 0.2 0
  4.57076 0.498688 0.767111 1 0.2 0
  0.100498 -2.93034 -0.82424 1 0.2 0
  0.187384 -4.47761 -1.13795 1 0.2 0
  -2.03032 1.61984 1.8209 1 0.2 0
  -3.12415 2.5086 2.5376 1 0.2 0
  -0.722716 -2.37912 -1.87868 1 0.2 0
  -0.956203 -3.63842 -2.80586 1 0.2 0
  -0.721396 -0.244718 2.66206 1 0.2 0
  -1.13175 -0.468164 4.17258 1 0.2 0
  0.872927 1.71137 2.86582 1 0.2 0
  1.38955 2.47062 4.15292 1 0.2 0
  0.322544 -0.697505 3.40762 1 0.2 0
  0.366759 -1.03433 4.95183 1 0.2 0
  -0.503356 1.60962 2.36293 1 0.2 0
  -0.707201 2.56708 3.6046 1 0.2 0
  -0.413829 0.258444 -3.27449 1 0.2 0
  -0.790577 0.45164 -4.79789 1 0.2 0
  -0.929782 2.84621 0.515796 1 0.2 0
  -1.37277 4.28691 0.993443 1 0.2 0
  -1.99447 1.7045 2.28319 1 0.2 0
  -3.02565 2.41935 3.2453 1 0.2 0
  2.10566 2.67809 1.02736 1 0.2 0
  3.09016 3.863 1.3834 1 0.2 0
  1.51959 2.95921 0.847587 1 0.2 0
  2.23438 4.30299 1.27578 1 0.2 0
  2.06279 2.43895 0.565247 1 0.2 0
  3.09563 3.6318 0.66704 1 0.2 0
  1.8212 -1.85939 -1.33707 1 0.2 0
  2.95299 -2.71806 -2.03115 1 0.2 0
  0.221785 1.09248 2.73786 1 0.2 0
  0.193612 1.70972 4.19327 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
  50 51 -1
  52 53 -1
  54 55 -1
  56 57 -1
  58 59 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 2
FrameBegin 2
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 82 neutrons
Color 1 0.2 0
XformPush
Translate -4.65297 -2.3297 -3.60704
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.14885 -1.82886 2.45281
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.05067 0.167243 -5.80959
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.89858 -4.96271 -3.00766
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.21447 2.81985 -3.03723
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.01819 -1.18177 -3.70053
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.96889 4.84913 -4.11378
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.98679 -4.5486 -4.17414
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.156942 0.170877 0.856934
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.224139 -1.76312 -0.40924
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.441248 -1.28993 6.03259
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.335588 5.9154 2.04806
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.84693 3.18523 4.97823
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.60322 5.62562 0.244586
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.11075 0.720062 1.04894
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.274269 -6.02488 -1.45165
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.21798 3.39735 3.2543
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.18969 -4.89772 -3.73304
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.410974 -1.37116 6.49604
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.911047 3.52453 4.84626
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.16733 0.644835 -6.32128
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.81576 5.72761 1.47109
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.05682 3.13421 4.20741
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.07465 5.04791 1.73945
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.94916 5.64677 1.70396
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.12847 4.82465 0.768832
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.08477 -3.57673 -2.72524
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.16544 2.32696 5.64868
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.54502 -0.680585 6.14423
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.50589 2.1502 -2.65824
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.86291 -1.47438 4.79932
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.71719 4.85106 -3.4716
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.96847 -0.646863 -2.01948
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.46752 2.16899 1.11538
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.68646 -2.1643 -1.29328
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.13243 -0.397123 -0.450345
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.71532 0.105492 -1.86501
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.82161 3.83287 0.641478
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.67242 3.37128 4.50178
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.465566 4.9869 -3.06117
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.39948 4.08454 -3.68551
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.63857 -6.36738 -1.21457
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.68902 3.9558 1.19791
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.12495 -2.95873 -3.34059
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.82567 -3.67645 4.83679
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.890603 -6.35487 0.181039
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.31655 0.77589 3.96117
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.05494 4.55553 3.78137
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.95569 -5.10918 -1.02402
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.97222 3.60075 3.65214
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.13485 -0.893276 0.646723
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.5213 1.52285 -1.58349
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.34024 0.906404 -3.66527
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.86495 5.93815 -1.16858
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.66059 3.53331 -1.26461
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.66263 -0.496078 2.87781
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.85955 3.56321 -0.373004
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.64752 -5.43091 -1.76869
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.79936 1.00643 2.04405
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.98102 -5.77331 1.08023
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.94496 -0.63455 -1.67728
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.36804 -2.09103 4.47365
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.781819 -4.8862 -3.17129
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.34619 -1.99693 3.43799
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.77755 -5.28825 -3.54617
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.00667 -3.90121 -3.40607
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.37545 5.46944 2.47174
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.92394 -1.96228 0.173085
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.13909 -0.588666 -0.469941
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.51177 -4.46088 1.17443
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.82124 0.447901 6.55191
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.02136 -4.67538 3.31145
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.74583 -1.57463 -5.20598
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.13905 2.71189 -5.32572
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.81608 -5.78112 1.14482
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.7825 1.3822 6.5166
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.88753 -3.28801 4.15356
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.334823 -0.0452317 1.08237
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.35146 -0.566306 6.04181
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 6 neutrons
Color 1 0.8 0
XformPush
Translate -2.70173 -1.26197 2.45351
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 2.83362 2.37438 2.01958
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.40968 -3.70802 -0.70187
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 2.13302 0.800768 -0.436053
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 2.54004 2.96617 2.17524
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 4.21478 -1.47905 -1.02703
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -1.40367 -2.01913 -1.54477
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.15986 0.832132 -1.40094
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
60 30
  -4.65297 -2.3297 -3.60704 1 0.2 0
  -5.81302 -2.95579 -4.48013 1 0.2 0
  -5.14885 -1.82886 2.45281 1 0.2 0
  -6.46221 -2.32219 3.182 1 0.2 0
  -1.05067 0.167243 -5.80959 1 0.2 0
  -1.31895 0.151029 -7.36772 1 0.2 0
  -1.89858 -4.96271 -3.00766 1 0.2 0
  -2.44839 -6.18459 -3.84714 1 0.2 0
  -4.21447 2.81985 -3.03723 1 0.2 0
  -5.35452 3.55346 -3.85092 1 0.2 0
  -5.01819 -1.18177 -3.70053 1 0.2 0
  -6.23364 -1.39736 -4.68856 1 0.2 0
  1.96889 4.84913 -4.11378 1 0.2 0
  2.43099 5.95654 -5.1434 1 0.2 0
  1.98679 -4.5486 -4.17414 1 0.2 0
  2.53168 -5.66135 -5.15643 1 0.2 0
  0.156942 0.170877 0.856934 1 0.2 0
  0.78441 -1.26434 1.07244 1 0.2 0
  0.224139 -1.76312 -0.40924 1 0.2 0
  -1.02725 -2.3677 0.344756 1 0.2 0
  -0.441248 -1.28993 6.03259 1 0.2 0
  -0.47226 -1.63529 7.57524 1 0.2 0
  0.335588 5.9154 2.04806 1 0.2 0
  0.478189 7.40335 2.56347 1 0.2 0
  -1.84693 3.18523 4.97823 1 0.2 0
  -2.30915 3.92225 6.29852 1 0.2 0
  -2.60322 5.62562 0.244586 1 0.2 0
  -3.24087 7.07223 0.271977 1 0.2 0
  6.11075 0.720062 1.04894 1 0.2 0
  7.65074 0.941437 1.33077 1 0.2 0
  0.274269 -6.02488 -1.45165 1 0.2 0
  0.361154 -7.57214 -1.76536 1 0.2 0
  -4.21798 3.39735 3.2543 1 0.2 0
  -5.3118 4.28611 3.971 1 0.2 0
  -1.18969 -4.89772 -3.73304 1 0.2 0
  -1.42318 -6.15701 -4.66021 1 0.2 0
  -2.70173 -1.26197 2.45351 1 0.8 0
  -3.69189 -1.7706 2.34923 1 0.8 0
  2.83362 2.37438 2.01958 1 0.8 0
  3.81396 2.70588 1.59646 1 0.8 0
  0.410974 -1.37116 6.49604 1 0.2 0
  0.455189 -1.70799 8.04025 1 0.2 0
  -0.911047 3.52453 4.84626 1 0.2 0
  -1.11489 4.48198 6.08793 1 0.2 0
  -1.16733 0.644835 -6.32128 1 0.2 0
  -1.54407 0.838031 -7.84468 1 0.2 0
  -1.81576 5.72761 1.47109 1 0.2 0
  -2.25874 7.16831 1.94874 1 0.2 0
  -4.05682 3.13421 4.20741 1 0.2 0
  -5.088 3.84906 5.16952 1 0.2 0
  4.07465 5.04791 1.73945 1 0.2 0
  5.05914 6.23282 2.09549 1 0.2 0
  2.94916 5.64677 1.70396 1 0.2 0
  3.66394 6.99055 2.13215 1 0.2 0
  4.12847 4.82465 0.768832 1 0.2 0
  5.16131 6.01749 0.870625 1 0.2 0
  4.08477 -3.57673 -2.72524 1 0.2 0
  5.21656 -4.4354 -3.41932 1 0.2 0
  0.16544 2.32696 5.64868 1 0.2 0
  0.137268 2.9442 7.10409 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
  50 51 -1
  52 53 -1
  54 55 -1
  56 57 -1
  58 59 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 3
FrameBegin 3
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 72 neutrons
Color 1 0.2 0
XformPush
Translate -6.97308 -3.58188 -5.35322
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -7.77556 -2.81552 3.91118
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.58722 0.134814 -8.92585
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.9982 -7.40647 -4.68661
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -7.44909 -1.61295 -5.67659
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.89309 7.06395 -6.17302
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.41188 -2.69955 1.28795
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.27864 -2.97228 1.09875
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.503272 -1.98066 9.11789
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.62079 8.89131 3.07889
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.490059 0.958943 6.7678
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.87851 8.51883 0.299368
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 9.19073 1.16281 1.6126
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.44804 -9.11941 -2.07906
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.40563 5.17486 4.6877
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.65666 -7.41631 -5.58739
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.499404 -2.04482 9.58447
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.92082 1.03123 -9.36808
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.70173 8.60901 2.42638
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.11917 4.56392 6.13164
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.04363 7.41772 2.45154
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.34834 -5.29406 -4.11341
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.109096 3.56144 8.5595
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.7728 -1.05794 9.03389
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 8.30614 3.06457 -3.80825
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.94797 -2.08537 7.09698
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -8.8961 -0.890656 -3.18974
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -8.31592 3.40803 1.70818
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.05876 -1.9282 -4.13247
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.80375 -0.431391 -1.44868
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 7.17804 5.88939 1.10834
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.86706 5.16655 6.81476
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.731769 7.64375 -4.75535
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.63146 6.13369 -5.7551
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.917773 -9.42703 -1.96328
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 7.06861 5.95644 1.77666
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.21606 -4.54263 -5.10651
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.37443 -9.46372 0.498788
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.67706 1.14148 2.01916
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.02831 -7.44011 -1.54463
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.96712 4.35904 6.55647
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.64575 -3.58108 1.34876
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.23885 2.67131 -4.44114
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.59048 -0.474828 -6.49892
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.81003 8.89289 -1.78204
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 8.52161 -0.629473 4.2226
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.04011 -0.787023 1.07196
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 7.46747 5.33507 -0.616484
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.62018 -8.30277 -2.66666
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.32974 1.80334 3.76512
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.99151 -8.68584 1.78454
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -8.98563 -1.14738 -2.37823
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.14965 -3.31997 6.7792
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.20052 -7.48129 -4.92918
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.67043 -3.05905 5.30073
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.45743 -7.90136 -5.1922
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.57792 -5.92906 -5.25512
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.93787 8.27588 3.81619
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.20167 -1.97477 3.25175
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.83691 -2.98498 3.55021
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.1564 -6.969 0.172152
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.68325 0.632379 9.58883
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.55287 -6.89607 4.96161
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.15321 -2.34623 -7.93066
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.74305 4.08355 -8.11027
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.59872 -8.81083 1.60125
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.03926 1.96855 9.61342
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.69681 -5.03677 6.06886
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.68711 1.32145 3.09333
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.862511 1.69124 6.08425
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 8 neutrons
Color 1 0.8 0
XformPush
Translate 2.83284 -6.32884 -5.23006
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 4.7943 3.03738 1.17334
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 2.08207 4.16882 0.150661
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.878867 2.64764 -0.563236
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 3.84566 4.30376 3.40252
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -0.58859 -1.84016 5.14929
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 2.4981 5.39951 1.78697
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.13689 -0.653632 0.532345
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 7 neutrons
Color 0 0.5 1
XformPush
Translate -2.60403 -1.72932 3.33216
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.576241 3.40144 3.91205
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.37593 -1.68638 -0.602162
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.57759 -4.5491 -1.21606
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.12357 0.772501 -0.40338
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.91597 3.5837 -0.599033
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.18464 -1.3601 -1.2397
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
60 30
  -6.97308 -3.58188 -5.35322 1 0.2 0
  -8.13313 -4.20797 -6.2263 1 0.2 0
  -7.77556 -2.81552 3.91118 1 0.2 0
  -9.08892 -3.30886 4.64037 1 0.2 0
  -1.58722 0.134814 -8.92585 1 0.2 0
  -1.8555 0.1186 -10.484 1 0.2 0
  -2.9982 -7.40647 -4.68661 1 0.2 0
  -3.54801 -8.62835 -5.52609 1 0.2 0
  -6.49457 4.28708 -4.66462 1 0.2 0
  -7.63462 5.02069 -5.47831 1 0.2 0
  -7.44909 -1.61295 -5.67659 1 0.2 0
  -8.66454 -1.82853 -6.66463 1 0.2 0
  2.89309 7.06395 -6.17302 1 0.2 0
  3.35519 8.17136 -7.20263 1 0.2 0
  2.83284 -6.32884 -5.23006 1 0.8 0
  3.25586 -7.21896 -5.75801 1 0.8 0
  1.41188 -2.69955 1.28795 1 0.2 0
  2.03935 -4.13477 1.50345 1 0.2 0
  -2.27864 -2.97228 1.09875 1 0.2 0
  -3.53002 -3.57687 1.85275 1 0.2 0
  -0.503272 -1.98066 9.11789 1 0.2 0
  -0.534285 -2.32602 10.6605 1 0.2 0
  0.62079 8.89131 3.07889 1 0.2 0
  0.763391 10.3793 3.59431 1 0.2 0
  -0.490059 0.958943 6.7678 1 0.2 0
  0.188377 -0.154201 7.66258 1 0.2 0
  -3.87851 8.51883 0.299368 1 0.2 0
  -4.51615 9.96544 0.326758 1 0.2 0
  9.19073 1.16281 1.6126 1 0.2 0
  10.7307 1.38419 1.89443 1 0.2 0
  0.44804 -9.11941 -2.07906 1 0.2 0
  0.534925 -10.6667 -2.39277 1 0.2 0
  -6.40563 5.17486 4.6877 1 0.2 0
  -7.49946 6.06362 5.4044 1 0.2 0
  -1.65666 -7.41631 -5.58739 1 0.2 0
  -1.89015 -8.6756 -6.51457 1 0.2 0
  -2.60403 -1.72932 3.33216 0 0.5 1
  -2.55518 -1.963 3.77149 0 0.5 1
  4.7943 3.03738 1.17334 1 0.8 0
  5.77465 3.36889 0.750216 1 0.8 0
  0.499404 -2.04482 9.58447 1 0.2 0
  0.543619 -2.38164 11.1287 1 0.2 0
  -0.576241 3.40144 3.91205 0 0.5 1
  -0.408837 3.3399 3.44494 0 0.5 1
  -1.92082 1.03123 -9.36808 1 0.2 0
  -2.29757 1.22442 -10.8915 1 0.2 0
  -2.70173 8.60901 2.42638 1 0.2 0
  -3.14472 10.0497 2.90403 1 0.2 0
  -6.11917 4.56392 6.13164 1 0.2 0
  -7.15035 5.27877 7.09375 1 0.2 0
  6.04363 7.41772 2.45154 1 0.2 0
  7.02812 8.60263 2.80758 1 0.2 0
  2.08207 4.16882 0.150661 1 0.8 0
  1.05887 3.84091 -0.158425 1 0.8 0
  6.34834 -5.29406 -4.11341 1 0.2 0
  7.48013 -6.15273 -4.80749 1 0.2 0
  0.109096 3.56144 8.5595 1 0.2 0
  0.0809237 4.17868 10.0149 1 0.2 0
  3.7728 -1.05794 9.03389 1 0.2 0
  4.38669 -1.24661 10.4787 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
  50 51 -1
  52 53 -1
  54 55 -1
  56 57 -1
  58 59 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 4
FrameBegin 4
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 22 neutrons
Color 1 0.2 0
XformPush
Translate -6.49457 4.28708 -4.66462
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.66682 -5.56998 1.71896
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.78141 -4.18145 2.60674
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.866813 -1.26734 8.55736
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.43106 -1.6921 -6.97167
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 7.89219 -0.968274 -1.03235
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.084592 0.256636 4.60394
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -7.15665 -6.26889 2.05081
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.95641 3.81977 -7.29879
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.84072 -1.85606 -9.33257
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.4493 -2.44547 5.77428
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.53888 -1.40181 -0.766026
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 7.86012 2.60025 5.4862
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.76816 -7.05152 -3.13405
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.47939 -1.98727 6.33041
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.13249 -5.79772 4.19049
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.70904 2.68813 5.1043
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.07648 3.94879 6.12668
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 8 neutrons
Color 1 0.8 0
XformPush
Translate 6.75499 3.70039 0.327096
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.0356703 3.51299 -0.467511
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -0.375286 4.49451 -0.69042
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 5.15129 5.64135 4.6298
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.648494 -0.00387001 5.4618
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 1.94126 6.24349 -0.207425
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.96119 0.676958 -1.06454
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.943685 3.25242 -6.19448
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 7 neutrons
Color 0 0.5 1
XformPush
Translate -2.50633 -2.19667 4.21082
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.163162 3.94396 4.64351
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.34818 -1.35362 0.340444
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.74551 -5.39018 -1.73024
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.08727 0.712871 0.59418
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.17134 3.63409 0.0665438
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.15449 -1.24115 -1.45238
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
60 30
  -6.49457 4.28708 -4.66462 1 0.2 0
  -5.14874 3.47273 -4.8245 1 0.2 0
  -6.49457 4.28708 -4.66462 1 0.2 0
  -5.1951 3.4071 -4.85696 1 0.2 0
  -3.88966 5.46174 -1.25633 1 0.2 0
  -3.25049 5.10989 -2.65907 1 0.2 0
  -3.88966 5.46174 -1.25633 1 0.2 0
  -4.68771 4.14566 -1.61834 1 0.2 0
  -3.88966 5.46174 -1.25633 1 0.2 0
  -5.16945 5.06551 -0.416604 1 0.2 0
  2.66682 -5.56998 1.71896 1 0.2 0
  3.29428 -7.0052 1.93447 1 0.2 0
  -4.78141 -4.18145 2.60674 1 0.2 0
  -6.0328 -4.78603 3.36074 1 0.2 0
  0.866813 -1.26734 8.55736 1 0.2 0
  1.54525 -2.38049 9.45214 1 0.2 0
  -2.50633 -2.19667 4.21082 0 0.5 1
  -2.45748 -2.43035 4.65015 0 0.5 1
  6.75499 3.70039 0.327096 1 0.8 0
  7.73534 4.03189 -0.0960248 1 0.8 0
  -0.163162 3.94396 4.64351 0 0.5 1
  0.0433768 4.21522 5.00925 0 0.5 1
  0.0356703 3.51299 -0.467511 1 0.8 0
  -0.98753 3.18508 -0.776596 1 0.8 0
  -1.34818 -1.35362 0.340444 0 0.5 1
  -1.33431 -1.18725 0.811747 0 0.5 1
  -2.74551 -5.39018 -1.73024 0 0.5 1
  -2.82946 -5.81072 -1.98733 0 0.5 1
  5.43106 -1.6921 -6.97167 1 0.2 0
  6.11721 -1.57405 -8.39127 1 0.2 0
  -0.375286 4.49451 -0.69042 1 0.8 0
  -1.00236 5.41794 -0.754012 1 0.8 0
  7.89219 -0.968274 -1.03235 1 0.2 0
  9.43641 -1.23672 -0.824188 1 0.2 0
  2.08727 0.712871 0.59418 0 0.5 1
  2.06913 0.683056 1.09296 0 0.5 1
  5.15129 5.64135 4.6298 1 0.8 0
  5.8041 6.31014 5.24343 1 0.8 0
  0.648494 -0.00387001 5.4618 1 0.8 0
  1.26704 0.914275 5.61806 1 0.8 0
  -0.084592 0.256636 4.60394 1 0.2 0
  0.711642 -0.185787 5.89633 1 0.2 0
  1.94126 6.24349 -0.207425 1 0.8 0
  1.66284 6.66548 -1.20462 1 0.8 0
  -7.15665 -6.26889 2.05081 1 0.2 0
  -7.9121 -7.6128 2.40183 1 0.2 0
  3.95641 3.81977 -7.29879 1 0.2 0
  4.31518 4.394 -8.72761 1 0.2 0
  -2.84072 -1.85606 -9.33257 1 0.2 0
  -2.96583 -2.54667 -10.7494 1 0.2 0
  3.17134 3.63409 0.0665438 0 0.5 1
  2.79903 3.65929 0.399332 0 0.5 1
  6.4493 -2.44547 5.77428 1 0.2 0
  5.41315 -3.35347 6.55012 1 0.2 0
  -6.53888 -1.40181 -0.766026 1 0.2 0
  -7.78826 -1.70921 -1.68502 1 0.2 0
  7.86012 2.60025 5.4862 1 0.2 0
  9.12531 2.9987 6.34674 1 0.2 0
  6.15449 -1.24115 -1.45238 0 0.5 1
  6.63942 -1.18168 -1.55871 0 0.5 1
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
  50 51 -1
  52 53 -1
  54 55 -1
  56 57 -1
  58 59 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 5
FrameBegin 5
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 12 neutrons
Color 1 0.2 0
XformPush
Translate -3.8029 2.65838 -4.98439
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.89562 2.52712 -5.04931
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.61132 4.75805 -4.0618
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.48576 2.82958 -1.98035
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.44924 4.66929 0.423122
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.92175 -8.44041 2.14997
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -7.28419 -5.39061 4.11473
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.50788 -0.62821 7.18873
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.377 -4.26148 7.32596
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -9.03765 -2.0166 -2.60401
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.33581 -6.62176 -1.33892
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.73097 4.05481 7.11526
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 6 neutrons
Color 1 0.8 0
XformPush
Translate 4.57555 3.36818 0.700848
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.01073 2.85717 -1.08568
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.62944 6.34137 -0.817604
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 1.88558 1.83242 5.77431
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 1.38442 7.08747 -2.20182
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.144325 2.42128 -4.2787
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 6 neutrons
Color 0 0.5 1
XformPush
Translate 0.249916 4.48648 5.37498
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.32044 -1.02087 1.28305
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.85981 -4.40749 -1.87604
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.05098 0.653241 1.59174
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.42671 3.68448 0.73212
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 7.12435 -1.1222 -1.66505
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
48 24
  -3.8029 2.65838 -4.98439 1 0.2 0
  -2.45706 1.84404 -5.14428 1 0.2 0
  -3.89562 2.52712 -5.04931 1 0.2 0
  -2.59614 1.64714 -5.24165 1 0.2 0
  -2.61132 4.75805 -4.0618 1 0.2 0
  -1.97216 4.4062 -5.46454 1 0.2 0
  -5.48576 2.82958 -1.98035 1 0.2 0
  -6.28381 1.5135 -2.34235 1 0.2 0
  -6.44924 4.66929 0.423122 1 0.2 0
  -7.72903 4.27306 1.26285 1 0.2 0
  3.92175 -8.44041 2.14997 1 0.2 0
  4.54922 -9.87562 2.36548 1 0.2 0
  -7.28419 -5.39061 4.11473 1 0.2 0
  -8.53557 -5.9952 4.86873 1 0.2 0
  4.57555 3.36818 0.700848 1 0.8 0
  3.48584 3.20208 0.887725 1 0.8 0
  0.249916 4.48648 5.37498 0 0.5 1
  0.456455 4.75774 5.74071 0 0.5 1
  -2.01073 2.85717 -1.08568 1 0.8 0
  -3.03393 2.52925 -1.39477 1 0.8 0
  -1.32044 -1.02087 1.28305 0 0.5 1
  -1.30656 -0.854493 1.75435 0 0.5 1
  -2.85981 -4.40749 -1.87604 0 0.5 1
  -2.91696 -3.91615 -1.94894 0 0.5 1
  -1.62944 6.34137 -0.817604 1 0.8 0
  -2.25652 7.26481 -0.881196 1 0.8 0
  2.05098 0.653241 1.59174 0 0.5 1
  2.03283 0.623426 2.09052 0 0.5 1
  1.88558 1.83242 5.77431 1 0.8 0
  2.50412 2.75057 5.93056 1 0.8 0
  1.50788 -0.62821 7.18873 1 0.2 0
  2.30411 -1.07063 8.48112 1 0.2 0
  1.38442 7.08747 -2.20182 1 0.8 0
  1.106 7.50947 -3.19902 1 0.8 0
  2.42671 3.68448 0.73212 0 0.5 1
  2.0544 3.70968 1.06491 0 0.5 1
  4.377 -4.26148 7.32596 1 0.2 0
  3.34085 -5.16948 8.1018 1 0.2 0
  -9.03765 -2.0166 -2.60401 1 0.2 0
  -10.287 -2.324 -3.523 1 0.2 0
  7.12435 -1.1222 -1.66505 0 0.5 1
  7.60928 -1.06273 -1.77138 0 0.5 1
  6.33581 -6.62176 -1.33892 1 0.2 0
  7.61963 -6.40688 -0.44135 1 0.2 0
  0.144325 2.42128 -4.2787 1 0.8 0
  -0.255355 2.00571 -3.32081 1 0.8 0
  5.73097 4.05481 7.11526 1 0.2 0
  6.74193 4.73815 8.12074 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 6
FrameBegin 6
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 3 neutrons
Color 1 0.2 0
XformPush
Translate -1.11123 1.02969 -5.30416
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.33299 4.05436 -6.86728
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -7.08186 0.197417 -2.70436
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 7 neutrons
Color 1 0.8 0
XformPush
Translate -2.71388 1.1533 -6.35931
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 2.39612 3.03598 1.0746
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -4.05713 2.20134 -1.70385
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.88359 8.18824 -0.944788
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 3.12266 3.66871 6.08682
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.82758 7.93146 -4.19622
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -0.655035 1.59014 -2.36291
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 5 neutrons
Color 0 0.5 1
XformPush
Translate -0.235545 5.33677 5.57828
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.29269 -0.688116 2.22566
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.97411 -3.4248 -2.02184
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.68209 3.73487 1.3977
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 8.0942 -1.00325 -1.87772
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
30 15
  -1.11123 1.02969 -5.30416 1 0.2 0
  0.234611 0.215345 -5.46405 1 0.2 0
  -2.71388 1.1533 -6.35931 1 0.8 0
  -2.12301 0.466384 -7.01431 1 0.8 0
  -1.33299 4.05436 -6.86728 1 0.2 0
  -0.69382 3.70252 -8.27001 1 0.2 0
  -7.08186 0.197417 -2.70436 1 0.2 0
  -7.87991 -1.11866 -3.06637 1 0.2 0
  2.39612 3.03598 1.0746 1 0.8 0
  1.3064 2.86987 1.26148 1 0.8 0
  -0.235545 5.33677 5.57828 0 0.5 1
  -0.478275 5.76192 5.67993 0 0.5 1
  -4.05713 2.20134 -1.70385 1 0.8 0
  -5.08033 1.87343 -2.01294 1 0.8 0
  -1.29269 -0.688116 2.22566 0 0.5 1
  -1.27882 -0.521739 2.69696 0 0.5 1
  -2.97411 -3.4248 -2.02184 0 0.5 1
  -3.03126 -2.93346 -2.09473 0 0.5 1
  -2.88359 8.18824 -0.944788 1 0.8 0
  -3.51067 9.11168 -1.00838 1 0.8 0
  3.12266 3.66871 6.08682 1 0.8 0
  3.7412 4.58686 6.24307 1 0.8 0
  0.82758 7.93146 -4.19622 1 0.8 0
  0.54916 8.35345 -5.19342 1 0.8 0
  1.68209 3.73487 1.3977 0 0.5 1
  1.30977 3.76007 1.73049 0 0.5 1
  8.0942 -1.00325 -1.87772 0 0.5 1
  8.57913 -0.943781 -1.98406 0 0.5 1
  -0.655035 1.59014 -2.36291 1 0.8 0
  -1.05472 1.17457 -1.40502 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 7
FrameBegin 7
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 2 neutrons
Color 1 0.2 0
XformPush
Translate 1.58045 -0.599001 -5.62394
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -8.67796 -2.43474 -3.42838
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 6 neutrons
Color 1 0.8 0
XformPush
Translate -1.53214 -0.220527 -7.66931
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.216679 2.70377 1.44835
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.10353 1.54552 -2.32203
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 4.35975 5.505 6.39933
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -0.695876 8.83804 -2.83343
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.4544 0.759007 -0.447126
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 5 neutrons
Color 0 0.5 1
XformPush
Translate -0.721005 6.18706 5.78158
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.26494 -0.355362 3.16826
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.08841 -2.44211 -2.16763
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.93746 3.78526 2.06327
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 9.06406 -0.884307 -2.09039
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
26 13
  1.58045 -0.599001 -5.62394 1 0.2 0
  2.92629 -1.41335 -5.78383 1 0.2 0
  -1.53214 -0.220527 -7.66931 1 0.8 0
  -0.941273 -0.907439 -8.32431 1 0.8 0
  -8.67796 -2.43474 -3.42838 1 0.2 0
  -9.47601 -3.75082 -3.79039 1 0.2 0
  0.216679 2.70377 1.44835 1 0.8 0
  -0.87304 2.53767 1.63523 1 0.8 0
  -0.721005 6.18706 5.78158 0 0.5 1
  -0.963735 6.61221 5.88323 0 0.5 1
  -6.10353 1.54552 -2.32203 1 0.8 0
  -7.12673 1.2176 -2.63111 1 0.8 0
  -1.26494 -0.355362 3.16826 0 0.5 1
  -1.25107 -0.188985 3.63956 0 0.5 1
  -3.08841 -2.44211 -2.16763 0 0.5 1
  -3.14556 -1.95077 -2.24053 0 0.5 1
  4.35975 5.505 6.39933 1 0.8 0
  4.97829 6.42315 6.55558 1 0.8 0
  -0.695876 8.83804 -2.83343 1 0.8 0
  -1.4576 9.29133 -2.15204 1 0.8 0
  0.93746 3.78526 2.06327 0 0.5 1
  0.565147 3.81046 2.39606 0 0.5 1
  9.06406 -0.884307 -2.09039 0 0.5 1
  9.54899 -0.824832 -2.19673 0 0.5 1
  -1.4544 0.759007 -0.447126 1 0.8 0
  -1.85408 0.343438 0.510767 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 8
FrameBegin 8
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 1 neutrons
Color 1 0.2 0
XformPush
Translate 4.27212 -2.22769 -5.94371
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 5 neutrons
Color 1 0.8 0
XformPush
Translate -0.350404 -1.59435 -8.97931
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.777384 4.1441 -0.16752
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 3.16628 4.10388 5.12948
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -0.848572 0.0614172 -2.48338
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 4 neutrons
Color 0 0.5 1
XformPush
Translate -1.20647 7.03735 5.98489
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.2372 -0.0226086 4.11087
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.20271 -1.45942 -2.31343
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.210456 9.12148 -3.14683
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
20 10
  4.27212 -2.22769 -5.94371 1 0.2 0
  5.61796 -3.04204 -6.1036 1 0.2 0
  -0.350404 -1.59435 -8.97931 1 0.8 0
  0.240466 -2.28126 -9.63431 1 0.8 0
  0.777384 4.1441 -0.16752 1 0.8 0
  1.05774 4.86427 -0.975457 1 0.8 0
  -1.20647 7.03735 5.98489 0 0.5 1
  -1.4492 7.4625 6.08654 0 0.5 1
  -8.14993 0.889689 -2.9402 1 0.8 0
  -9.17313 0.561776 -3.24928 1 0.8 0
  -1.2372 -0.0226086 4.11087 0 0.5 1
  -1.22332 0.143768 4.58217 0 0.5 1
  -3.20271 -1.45942 -2.31343 0 0.5 1
  -3.25986 -0.96808 -2.38633 0 0.5 1
  3.16628 4.10388 5.12948 1 0.8 0
  2.56955 3.40332 4.49456 1 0.8 0
  0.210456 9.12148 -3.14683 0 0.5 1
  0.663622 9.2632 -3.30354 0 0.5 1
  -0.848572 0.0614172 -2.48338 1 0.8 0
  -0.54566 -0.287377 -3.50151 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 9
FrameBegin 9
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 3 neutrons
Color 1 0.2 0
XformPush
Translate 1.8556 -0.601603 -7.17507
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 3 neutrons
Color 1 0.8 0
XformPush
Translate 1.33809 5.58444 -1.78339
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 1.97282 2.70277 3.85963
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -0.242748 -0.636172 -4.51964
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -1.20945 0.310145 5.05347
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.31701 -0.476735 -2.45923
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
16 8
  1.8556 -0.601603 -7.17507 1 0.2 0
  0.647337 0.211442 -7.79075 1 0.2 0
  -8.14993 0.889689 -2.9402 1 0.2 0
  -8.22699 2.34789 -3.54659 1 0.2 0
  -8.14993 0.889689 -2.9402 1 0.2 0
  -8.49389 2.37476 -3.36004 1 0.2 0
  1.33809 5.58444 -1.78339 1 0.8 0
  1.61844 6.30461 -2.59133 1 0.8 0
  -1.20945 0.310145 5.05347 0 0.5 1
  -1.19558 0.476522 5.52477 0 0.5 1
  -3.31701 -0.476735 -2.45923 0 0.5 1
  -3.37416 0.0146097 -2.53212 0 0.5 1
  1.97282 2.70277 3.85963 1 0.8 0
  1.37608 2.00221 3.22471 1 0.8 0
  -0.242748 -0.636172 -4.51964 1 0.8 0
  0.0601633 -0.984967 -5.53777 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 10
FrameBegin 10
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.068936 -0.0314335 0.24339
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.250798 0.278301 0.231973
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.268981 0.209517 0.322969
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 1 neutrons
Color 1 0.2 0
XformPush
Translate -0.560925 1.02449 -8.40643
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 3 neutrons
Color 1 0.8 0
XformPush
Translate 1.89879 7.02477 -3.39927
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.779351 1.30165 2.58979
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.363075 -1.33376 -6.5559
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -1.18171 0.642899 5.99608
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.43131 0.505954 -2.60502
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
12 6
  -0.560925 1.02449 -8.40643 1 0.2 0
  -1.76919 1.83753 -9.02211 1 0.2 0
  1.89879 7.02477 -3.39927 1 0.8 0
  2.17915 7.74494 -4.20721 1 0.8 0
  -1.18171 0.642899 5.99608 0 0.5 1
  -1.16783 0.809276 6.46738 0 0.5 1
  -3.43131 0.505954 -2.60502 0 0.5 1
  -3.48847 0.997299 -2.67792 0 0.5 1
  0.779351 1.30165 2.58979 1 0.8 0
  0.182618 0.601089 1.95486 1 0.8 0
  0.363075 -1.33376 -6.5559 1 0.8 0
  0.665987 -1.68256 -7.57403 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 11
FrameBegin 11
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -2.79213 0.424457 -0.851813
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.54134 -0.172234 2.90994
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 1 - 2 neutrons
Color 1 0.8 0
XformPush
Translate -0.414115 -0.0994701 1.31994
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate 0.968898 -2.03135 -8.59216
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -2.09947 0.297263 5.80053
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.54562 1.48864 -2.75082
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
8 4
  -2.09947 0.297263 5.80053 0 0.5 1
  -2.55836 0.124445 5.70275 0 0.5 1
  -3.54562 1.48864 -2.75082 0 0.5 1
  -3.60277 1.97999 -2.82372 0 0.5 1
  -0.414115 -0.0994701 1.31994 1 0.8 0
  -1.01085 -0.800029 0.685019 1 0.8 0
  0.968898 -2.03135 -8.59216 1 0.8 0
  1.27181 -2.38015 -9.61029 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 12
FrameBegin 12
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 1 - 1 neutrons
Color 1 0.8 0
XformPush
Translate -1.60758 -1.50059 0.0500958
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -2.72401 -0.473099 5.67211
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.65992 2.47133 -2.89662
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
6 3
  -2.72401 -0.473099 5.67211 0 0.5 1
  -3.03628 -0.85828 5.60791 0 0.5 1
  -3.65992 2.47133 -2.89662 0 0.5 1
  -3.71707 2.96268 -2.96951 0 0.5 1
  -1.60758 -1.50059 0.0500958 1 0.8 0
  -2.20431 -2.20115 -0.584827 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 13
FrameBegin 13
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -6.49457 4.28708 -4.66462
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.88966 5.46174 -1.25633
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 1 - 1 neutrons
Color 1 0.8 0
XformPush
Translate -2.80105 -2.90171 -1.21975
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -3.34856 -1.24346 5.5437
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.77422 3.45402 -3.04241
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
6 3
  -3.34856 -1.24346 5.5437 0 0.5 1
  -3.66083 -1.62864 5.47949 0 0.5 1
  -3.77422 3.45402 -3.04241 0 0.5 1
  -3.83137 3.94537 -3.11531 0 0.5 1
  -2.80105 -2.90171 -1.21975 1 0.8 0
  -3.39778 -3.60226 -1.85467 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 14
FrameBegin 14
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 1 - 1 neutrons
Color 1 0.8 0
XformPush
Translate -3.99451 -4.30282 -2.4896
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -3.9731 -2.01382 5.41528
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.88852 4.43671 -3.18821
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
6 3
  -3.9731 -2.01382 5.41528 0 0.5 1
  -4.28537 -2.399 5.35108 0 0.5 1
  -3.88852 4.43671 -3.18821 0 0.5 1
  -3.94567 4.92806 -3.26111 0 0.5 1
  -3.99451 -4.30282 -2.4896 1 0.8 0
  -4.59124 -5.00338 -3.12452 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 15
FrameBegin 15
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 1 - 1 neutrons
Color 1 0.8 0
XformPush
Translate -5.18798 -5.70394 -3.75944
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -4.59764 -2.78418 5.28687
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.00282 5.4194 -3.33401
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
6 3
  -4.59764 -2.78418 5.28687 0 0.5 1
  -4.90991 -3.16937 5.22266 0 0.5 1
  -4.00282 5.4194 -3.33401 0 0.5 1
  -4.05997 5.91075 -3.4069 0 0.5 1
  -5.18798 -5.70394 -3.75944 1 0.8 0
  -5.78471 -6.4045 -4.39437 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 16
FrameBegin 16
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -5.22218 -3.55455 5.15845
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.11712 6.40209 -3.4798
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
4 2
  -5.22218 -3.55455 5.15845 0 0.5 1
  -5.53445 -3.93973 5.09425 0 0.5 1
  -4.11712 6.40209 -3.4798 0 0.5 1
  -4.17427 6.89344 -3.5527 0 0.5 1
  0 1 -1
  2 3 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 17
FrameBegin 17
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 2 neutrons
Color 0 0.5 1
XformPush
Translate -5.84672 -4.32491 5.03004
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.23142 7.38478 -3.6256
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
4 2
  -5.84672 -4.32491 5.03004 0 0.5 1
  -6.15899 -4.71009 4.96583 0 0.5 1
  -4.23142 7.38478 -3.6256 0 0.5 1
  -4.28858 7.87613 -3.6985 0 0.5 1
  0 1 -1
  2 3 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 18
FrameBegin 18
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -8.14993 0.889689 -2.9402
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -6.47126 -5.09527 4.90162
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -6.47126 -5.09527 4.90162 0 0.5 1
  -6.78354 -5.48045 4.83742 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 19
FrameBegin 19
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
LineSet "PC"
0 0
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 20
FrameBegin 20
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 25 neutrons
Color 1 0.2 0
XformPush
Translate 0.88798 -1.77612 -2.73188
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.70311 -1.17141 -0.0166692
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.19004 -0.810788 2.18703
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.455989 -0.73368 2.86044
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.56536 0.0604984 3.16451
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.3785 -0.301788 0.535784
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.27286 0.674767 -1.65261
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.846714 -0.0936732 2.67166
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.409388 -0.671251 -2.68212
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.0300987 2.98562 -0.425274
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.842952 -1.78295 2.13937
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.249369 -2.78291 -1.23695
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.4221 1.77108 -0.215278
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.0294257 0.261517 -0.120409
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.786 -0.958456 -2.60734
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.30731 -1.34553 0.476608
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.84735 1.44273 0.00252435
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.597605 0.563966 -2.99312
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.38518 0.301511 -0.125118
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.407531 -1.01253 -3.07845
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.65448 0.306752 2.70284
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.420919 -1.24553 -2.58898
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.98567 -0.362266 -2.07681
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.92839 2.67441 -0.733462
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.82248 -0.644764 -2.51814
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 2 neutrons
Color 1 0.8 0
XformPush
Translate 0.681214 0.512237 -2.19917
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -0.774491 -1.41732 1.15489
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
LineSet "PC"
54 27
  0.88798 -1.77612 -2.73188 1 0.2 0
  1.29247 -2.6132 -4.01082 1 0.2 0
  -2.70311 -1.17141 -0.0166692 1 0.2 0
  -4.15106 -1.77452 0.182589 1 0.2 0
  2.19004 -0.810788 2.18703 1 0.2 0
  3.37819 -1.00489 3.21203 1 0.2 0
  -0.455989 -0.73368 2.86044 1 0.2 0
  -0.69904 -0.90893 4.41293 1 0.2 0
  1.56536 0.0604984 3.16451 1 0.2 0
  2.25398 0.216033 4.5793 1 0.2 0
  0.681214 0.512237 -2.19917 1 0.8 0
  0.852105 0.766066 -3.27452 1 0.8 0
  3.3785 -0.301788 0.535784 1 0.2 0
  4.9397 -0.511116 0.673118 1 0.2 0
  -2.27286 0.674767 -1.65261 1 0.2 0
  -3.50039 0.943074 -2.61239 1 0.2 0
  0.846714 -0.0936732 2.67166 1 0.2 0
  1.41066 -0.254142 4.14007 1 0.2 0
  0.409388 -0.671251 -2.68212 1 0.2 0
  0.662561 -1.12269 -4.17614 1 0.2 0
  -0.0300987 2.98562 -0.425274 1 0.2 0
  -0.0540596 4.54888 -0.661171 1 0.2 0
  -0.842952 -1.78295 2.13937 1 0.2 0
  -1.39271 -2.61755 3.3646 1 0.2 0
  0.249369 -2.78291 -1.23695 1 0.2 0
  0.324491 -4.11178 -2.09046 1 0.2 0
  -2.4221 1.77108 -0.215278 1 0.2 0
  -3.71635 2.67846 -0.175842 1 0.2 0
  -0.0294257 0.261517 -0.120409 1 0.2 0
  -0.0503029 1.7872 0.294148 1 0.2 0
  -1.786 -0.958456 -2.60734 1 0.2 0
  -2.62556 -1.57312 -3.79786 1 0.2 0
  2.30731 -1.34553 0.476608 1 0.2 0
  3.59697 -2.22533 0.72708 1 0.2 0
  2.84735 1.44273 0.00252435 1 0.2 0
  4.27958 2.10066 0.128425 1 0.2 0
  0.597605 0.563966 -2.99312 1 0.2 0
  0.719877 0.710541 -4.56269 1 0.2 0
  3.38518 0.301511 -0.125118 1 0.2 0
  4.93705 0.507831 -0.346754 1 0.2 0
  -0.407531 -1.01253 -3.07845 1 0.2 0
  -0.68325 -1.67568 -4.48708 1 0.2 0
  1.65448 0.306752 2.70284 1 0.2 0
  2.53932 0.581963 3.98398 1 0.2 0
  -0.420919 -1.24553 -2.58898 1 0.2 0
  -0.713753 -2.09576 -3.8895 1 0.2 0
  2.98567 -0.362266 -2.07681 1 0.2 0
  4.29319 -0.510329 -2.95344 1 0.2 0
  -1.92839 2.67441 -0.733462 1 0.2 0
  -2.89052 3.81777 -1.2502 1 0.2 0
  -0.774491 -1.41732 1.15489 1 0.8 0
  -1.14702 -2.25673 1.79254 1 0.8 0
  1.82248 -0.644764 -2.51814 1 0.2 0
  2.74843 -1.0979 -3.717 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
  50 51 -1
  52 53 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 21
FrameBegin 21
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 22 neutrons
Color 1 0.2 0
XformPush
Translate 1.69697 -3.45028 -5.28975
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.59901 -2.37762 0.381847
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.56634 -1.19899 4.23703
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.942092 -1.08418 5.96541
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.94259 0.371567 5.99408
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.50089 -0.720443 0.810452
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.72792 1.21138 -3.57217
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.9746 -0.41461 5.60848
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.915733 -1.57413 -5.67016
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.0780204 6.11214 -0.897068
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.399613 -5.44064 -2.94396
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.0711801 3.31288 0.708705
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.46511 -2.18778 -4.98838
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.594699 -2.52227 0.916631
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.842149 0.857117 -6.13227
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.48893 0.714152 -0.56839
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.95897 -2.33883 -5.8957
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.42416 0.857174 5.26512
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.00659 -2.946 -5.19002
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.60071 -0.658392 -3.83007
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.67438 -1.55105 -4.91587
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 3 neutrons
Color 1 0.8 0
XformPush
Translate 1.023 1.01989 -4.34986
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.64441 3.39259 0.50731
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -1.51956 -3.09615 2.43019
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
LineSet "PC"
50 25
  1.69697 -3.45028 -5.28975 1 0.2 0
  2.10146 -4.28737 -6.56869 1 0.2 0
  -5.59901 -2.37762 0.381847 1 0.2 0
  -7.04696 -2.98073 0.581106 1 0.2 0
  4.56634 -1.19899 4.23703 1 0.2 0
  5.75449 -1.3931 5.26203 1 0.2 0
  -0.942092 -1.08418 5.96541 1 0.2 0
  -1.18514 -1.25943 7.5179 1 0.2 0
  2.94259 0.371567 5.99408 1 0.2 0
  3.6312 0.527101 7.40887 1 0.2 0
  1.023 1.01989 -4.34986 1 0.8 0
  1.19389 1.27372 -5.42521 1 0.8 0
  6.50089 -0.720443 0.810452 1 0.2 0
  8.06208 -0.92977 0.947786 1 0.2 0
  -4.72792 1.21138 -3.57217 1 0.2 0
  -5.95545 1.47969 -4.53196 1 0.2 0
  1.9746 -0.41461 5.60848 1 0.2 0
  2.53854 -0.575079 7.07688 1 0.2 0
  0.915733 -1.57413 -5.67016 1 0.2 0
  1.16891 -2.02557 -7.16419 1 0.2 0
  -0.0780204 6.11214 -0.897068 1 0.2 0
  -0.101981 7.6754 -1.13297 1 0.2 0
  0.399613 -5.44064 -2.94396 1 0.2 0
  0.474735 -6.7695 -3.79747 1 0.2 0
  -5.01061 3.58584 -0.136406 1 0.2 0
  -6.30487 4.49322 -0.09697 1 0.2 0
  -0.0711801 3.31288 0.708705 1 0.2 0
  -0.0920574 4.83856 1.12326 1 0.2 0
  -3.46511 -2.18778 -4.98838 1 0.2 0
  -4.30467 -2.80244 -6.1789 1 0.2 0
  -0.594699 -2.52227 0.916631 1 0.2 0
  -2.0457 -3.11064 1.13664 1 0.2 0
  0.842149 0.857117 -6.13227 1 0.2 0
  0.964422 1.00369 -7.70184 1 0.2 0
  6.48893 0.714152 -0.56839 1 0.2 0
  8.0408 0.920472 -0.790026 1 0.2 0
  -0.95897 -2.33883 -5.8957 1 0.2 0
  -1.23469 -3.00197 -7.30432 1 0.2 0
  3.42416 0.857174 5.26512 1 0.2 0
  4.309 1.13239 6.54626 1 0.2 0
  -1.00659 -2.946 -5.19002 1 0.2 0
  -1.29942 -3.79623 -6.49054 1 0.2 0
  5.60071 -0.658392 -3.83007 1 0.2 0
  6.90822 -0.806455 -4.7067 1 0.2 0
  -3.64441 3.39259 0.50731 1 0.8 0
  -4.50241 3.75168 1.1277 1 0.8 0
  -1.51956 -3.09615 2.43019 1 0.8 0
  -1.89209 -3.93557 3.06784 1 0.8 0
  3.67438 -1.55105 -4.91587 1 0.2 0
  4.60033 -2.00419 -6.11473 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 22
FrameBegin 22
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 25 neutrons
Color 1 0.2 0
XformPush
Translate 2.50595 -5.12445 -7.84762
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -8.49491 -3.58384 0.780364
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 6.94264 -1.5872 6.28703
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.4282 -1.43468 9.07039
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 4.31981 0.682635 8.82366
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 9.62327 -1.1391 1.08512
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -7.18298 1.74799 -5.49174
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.10248 -0.735548 8.54529
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.42208 -2.477 -8.65821
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.125942 9.23866 -1.36886
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.549857 -8.09837 -4.65098
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.112935 6.36425 1.53782
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.14423 -3.4171 -7.36942
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.49671 -3.69901 1.35665
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.08669 1.15027 -9.27142
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 9.59267 1.12679 -1.01166
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.51041 -3.66512 -8.71294
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.19384 1.4076 7.8274
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.59225 -4.64647 -7.79106
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 8.21574 -0.954518 -5.58332
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 5.52628 -2.45733 -7.3136
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.35055 3.41709 2.72298
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 3 neutrons
Color 1 0.8 0
XformPush
Translate 1.36478 1.52755 -6.50055
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.36042 4.11077 1.74808
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -2.26462 -4.77499 3.70549
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
LineSet "PC"
56 28
  2.50595 -5.12445 -7.84762 1 0.2 0
  2.91045 -5.96153 -9.12656 1 0.2 0
  -8.49491 -3.58384 0.780364 1 0.2 0
  -9.94286 -4.18694 0.979622 1 0.2 0
  6.94264 -1.5872 6.28703 1 0.2 0
  8.13079 -1.7813 7.31203 1 0.2 0
  -1.4282 -1.43468 9.07039 1 0.2 0
  -1.67125 -1.60993 10.6229 1 0.2 0
  4.31981 0.682635 8.82366 1 0.2 0
  5.00843 0.83817 10.2384 1 0.2 0
  1.36478 1.52755 -6.50055 1 0.8 0
  1.53567 1.78138 -7.5759 1 0.8 0
  9.62327 -1.1391 1.08512 1 0.2 0
  11.1845 -1.34843 1.22245 1 0.2 0
  -7.18298 1.74799 -5.49174 1 0.2 0
  -8.41051 2.0163 -6.45153 1 0.2 0
  3.10248 -0.735548 8.54529 1 0.2 0
  3.66642 -0.896016 10.0137 1 0.2 0
  1.42208 -2.477 -8.65821 1 0.2 0
  1.67525 -2.92844 -10.1522 1 0.2 0
  -0.125942 9.23866 -1.36886 1 0.2 0
  -0.149903 10.8019 -1.60476 1 0.2 0
  -5.01061 3.58584 -0.136406 1 0.2 0
  -6.51971 4.04981 -0.0506238 1 0.2 0
  0.549857 -8.09837 -4.65098 1 0.2 0
  0.624979 -9.42723 -5.50449 1 0.2 0
  -5.01061 3.58584 -0.136406 1 0.2 0
  -4.6881 4.28111 -1.51937 1 0.2 0
  -0.112935 6.36425 1.53782 1 0.2 0
  -0.133812 7.88993 1.95238 1 0.2 0
  -5.14423 -3.4171 -7.36942 1 0.2 0
  -5.98379 -4.03176 -8.55994 1 0.2 0
  -3.49671 -3.69901 1.35665 1 0.2 0
  -4.94771 -4.28738 1.57667 1 0.2 0
  -5.01061 3.58584 -0.136406 1 0.2 0
  -6.33214 4.2433 -0.703256 1 0.2 0
  1.08669 1.15027 -9.27142 1 0.2 0
  1.20897 1.29684 -10.841 1 0.2 0
  9.59267 1.12679 -1.01166 1 0.2 0
  11.1445 1.33311 -1.2333 1 0.2 0
  -1.51041 -3.66512 -8.71294 1 0.2 0
  -1.78613 -4.32826 -10.1216 1 0.2 0
  5.19384 1.4076 7.8274 1 0.2 0
  6.07868 1.68281 9.10854 1 0.2 0
  -1.59225 -4.64647 -7.79106 1 0.2 0
  -1.88509 -5.4967 -9.09158 1 0.2 0
  8.21574 -0.954518 -5.58332 1 0.2 0
  9.52326 -1.10258 -6.45995 1 0.2 0
  -5.36042 4.11077 1.74808 1 0.8 0
  -6.21843 4.46986 2.36847 1 0.8 0
  -2.26462 -4.77499 3.70549 1 0.8 0
  -2.63716 -5.6144 4.34314 1 0.8 0
  5.52628 -2.45733 -7.3136 1 0.2 0
  6.45224 -2.91047 -8.51246 1 0.2 0
  -6.35055 3.41709 2.72298 1 0.2 0
  -7.02051 3.33272 4.15268 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
  50 51 -1
  52 53 -1
  54 55 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 23
FrameBegin 23
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.9 0.72 0.18
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.75 0.75 0.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 6 neutrons
Color 1 0.2 0
XformPush
Translate -8.02881 4.51377 0.0351583
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -4.36559 4.97638 -2.90233
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.154689 9.41561 2.36694
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -6.39872 -4.87575 1.79668
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -7.65366 4.90077 -1.27011
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.49917 -2.8392 -8.14472
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 4 neutrons
Color 1 0.8 0
XformPush
Translate 1.70656 2.03521 -8.65124
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -7.07644 4.82895 2.98885
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -3.00969 -6.45382 4.98079
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -8.00064 2.7266 4.06478
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
LineSet "PC"
20 10
  1.70656 2.03521 -8.65124 1 0.8 0
  1.87745 2.28904 -9.72659 1 0.8 0
  -8.02881 4.51377 0.0351583 1 0.2 0
  -9.53791 4.97774 0.12094 1 0.2 0
  -4.36559 4.97638 -2.90233 1 0.2 0
  -4.04307 5.67165 -4.28529 1 0.2 0
  -0.154689 9.41561 2.36694 1 0.2 0
  -0.175566 10.9413 2.78149 1 0.2 0
  -6.39872 -4.87575 1.79668 1 0.2 0
  -7.84972 -5.46412 2.01669 1 0.2 0
  -7.65366 4.90077 -1.27011 1 0.2 0
  -8.97518 5.55823 -1.83696 1 0.2 0
  -7.07644 4.82895 2.98885 1 0.8 0
  -7.93444 5.18804 3.60924 1 0.8 0
  -3.00969 -6.45382 4.98079 1 0.8 0
  -3.38222 -7.29324 5.61844 1 0.8 0
  2.49917 -2.8392 -8.14472 1 0.2 0
  0.985613 -3.03013 -8.56027 1 0.2 0
  -8.00064 2.7266 4.06478 1 0.8 0
  -8.82569 2.38136 4.73567 1 0.8 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 24
FrameBegin 24
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.8 0.64 0.16
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1 1 1
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 2 neutrons
Color 1 0.2 0
XformPush
Translate -3.72056 6.36692 -5.66826
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.527944 -3.22107 -8.97583
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
# Energy Group 1 - 1 neutrons
Color 1 0.8 0
XformPush
Translate -4.85529 5.04868 2.85382
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
LineSet "PC"
6 3
  -3.72056 6.36692 -5.66826 1 0.2 0
  -3.39804 7.06219 -7.05122 1 0.2 0
  -4.85529 5.04868 2.85382 1 0.8 0
  -3.74472 5.15854 2.78631 1 0.8 0
  -0.527944 -3.22107 -8.97583 1 0.2 0
  -2.0415 -3.412 -9.39139 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 25
FrameBegin 25
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.7 0.56 0.14
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.25 1.25 1.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 1 - 1 neutrons
Color 1 0.8 0
XformPush
Translate -2.63414 5.26841 2.71879
Scale 0.17 0.17 0.17
OptionBool "Wireframe" true
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
LineSet "PC"
2 1
  -2.63414 5.26841 2.71879 1 0.8 0
  -1.52357 5.37827 2.65128 1 0.8 0
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 26
FrameBegin 26
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.6 0.48 0.12
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.5 1.5 1.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -2.91919 4.31042 2.75066
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -2.91919 4.31042 2.75066 0 0.5 1
  -3.06171 3.83143 2.7666 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 27
FrameBegin 27
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.5 0.4 0.1
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 1.75 1.75 1.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -1.98148 4.65631 2.71809
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -1.98148 4.65631 2.71809 0 0.5 1
  -1.51262 4.82925 2.7018 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 28
FrameBegin 28
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.4 0.32 0.08
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2 2 2
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -1.04376 5.00219 2.68552
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -1.04376 5.00219 2.68552 0 0.5 1
  -0.574907 5.17513 2.66923 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 29
FrameBegin 29
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.0294257 0.261517 -0.120409
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.3 0.24 0.06
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.25 2.25 2.25
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -0.106051 5.34807 2.65295
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -0.106051 5.34807 2.65295 0 0.5 1
  0.362806 5.52101 2.63666 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 30
FrameBegin 30
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.2 0.16 0.04
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.5 2.5 2.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -0.642388 5.99995 3.18903
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -0.642388 5.99995 3.18903 0 0.5 1
  -0.910556 6.3259 3.45707 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 31
FrameBegin 31
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Translate -5.01061 3.58584 -0.136406
Color 0.1 0.08 0.02
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 2.75 2.75 2.75
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -1.17872 6.65184 3.72511
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -1.17872 6.65184 3.72511 0 0.5 1
  -1.44689 6.97778 3.99316 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 32
FrameBegin 32
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -1.71506 7.30373 4.2612
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -1.71506 7.30373 4.2612 0 0.5 1
  -1.98323 7.62967 4.52924 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 33
FrameBegin 33
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
# Energy Group 2 - 1 neutrons
Color 0 0.5 1
XformPush
Translate -2.2514 7.95561 4.79728
Scale 0.14 0.14 0.14
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
2 1
  -2.2514 7.95561 4.79728 0 0.5 1
  -2.51957 8.28156 5.06532 0 0.5 1
  0 1 -1
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 34
FrameBegin 34
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
LineSet "PC"
0 0
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 35
FrameBegin 35
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
LineSet "PC"
0 0
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 36
FrameBegin 36
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
LineSet "PC"
0 0
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 37
FrameBegin 37
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
LineSet "PC"
0 0
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 38
FrameBegin 38
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
LineSet "PC"
0 0
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 39
FrameBegin 39
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
LineSet "PC"
0 0
WorldEnd
FrameEnd

# Neutron Diffusion Simulation - Frame 40
FrameBegin 40
WorldBegin
AmbientLight 0.4 0.4 0.5 1.0
FarLight 1 1 1 1.0 1.0 1.0 1.5
PointLight 0 10 0 1.0 1.0 1.0 2.0
XformPush
Color 0.3 0.3 0.3
Surface "plastic"
Translate 0 -10 0
Scale 20 1 20
Cube
XformPop

XformPush
Color 0.3 0.8 0.3
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.3
OptionBool "Wireframe" true
Scale 3 3 3
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
XformPush
Color 1.0 1.0 0.0
Surface "metal"
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
XformPop

Surface "plastic"
XformPush
Translate -0.374395 -0.159507 -0.105163
Color 1 0.8 0.2
Surface "plastic"
Ka 0.8
Kd 0.8
Ks 0.5
OptionBool "Wireframe" true
Scale 0.5 0.5 0.5
Sphere 1.0 -1.0 1.0 360
OptionBool "Wireframe" false
XformPop
# Energy Group 0 - 26 neutrons
Color 1 0.2 0
XformPush
Translate -0.767444 2.6877 0.210583
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.97652 -2.51239 1.06498
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.451408 -2.07607 -2.27546
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.221587 3.51792 -0.109023
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.256729 1.40637 3.03198
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.14115 -1.85199 -0.914854
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.319637 2.40379 -1.28813
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.24509 -1.98243 2.43564
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.15487 2.51415 -1.19147
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -2.78733 0.449976 -0.0871732
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.26237 2.11862 1.56242
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.17358 -1.98955 -1.10236
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.15845 -3.37129 0.628296
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.26865 -2.1822 1.95024
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.666844 -2.22046 1.59471
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.837811 -1.59062 -2.07956
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 1.82985 0.871224 -1.88647
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -3.29433 -1.29862 -0.391632
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.162023 1.82744 -2.32202
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.29219 2.05831 -2.0948
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.0832456 -3.07112 -0.0825312
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 2.84351 1.21969 -0.748027
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 3.24525 0.112649 0.853299
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -1.33731 -1.79526 2.62888
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate -0.374395 -0.159507 -0.105163
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
XformPush
Translate 0.619218 -2.52038 1.74929
Scale 0.2 0.2 0.2
Sphere 1.0 -1.0 1.0 360
XformPop
LineSet "PC"
52 26
  -0.767444 2.6877 0.210583 1 0.2 0
  -1.09862 4.21353 0.459969 1 0.2 0
  -1.97652 -2.51239 1.06498 1 0.2 0
  -2.96242 -3.58577 1.67804 1 0.2 0
  -0.451408 -2.07607 -2.27546 1 0.2 0
  -0.817662 -3.06701 -3.45184 1 0.2 0
  -0.221587 3.51792 -0.109023 1 0.2 0
  -0.250068 5.09686 -0.0306899 1 0.2 0
  -0.256729 1.40637 3.03198 1 0.2 0
  -0.237716 2.05488 4.47388 1 0.2 0
  -2.14115 -1.85199 -0.914854 1 0.2 0
  -3.14438 -2.99355 -1.35117 1 0.2 0
  -0.319637 2.40379 -1.28813 1 0.2 0
  -0.373012 3.80933 -2.01035 1 0.2 0
  1.24509 -1.98243 2.43564 1 0.2 0
  1.70806 -2.99443 3.5588 1 0.2 0
  2.15487 2.51415 -1.19147 1 0.2 0
  3.19592 3.52831 -1.81413 1 0.2 0
  -2.78733 0.449976 -0.0871732 1 0.2 0
  -4.35202 0.670322 -0.14353 1 0.2 0
  1.26237 2.11862 1.56242 1 0.2 0
  2.00227 3.18834 2.46144 1 0.2 0
  2.17358 -1.98955 -1.10236 1 0.2 0
  3.20142 -3.09176 -1.58057 1 0.2 0
  -0.15845 -3.37129 0.628296 1 0.2 0
  -0.228276 -4.94273 0.788663 1 0.2 0
  -1.26865 -2.1822 1.95024 1 0.2 0
  -1.6935 -3.23836 3.04752 1 0.2 0
  -0.666844 -2.22046 1.59471 1 0.2 0
  -1.02912 -3.49896 2.45155 1 0.2 0
  -0.837811 -1.59062 -2.07956 1 0.2 0
  -1.28144 -2.62023 -3.19451 1 0.2 0
  1.82985 0.871224 -1.88647 1 0.2 0
  2.96565 1.39005 -2.85641 1 0.2 0
  -3.29433 -1.29862 -0.391632 1 0.2 0
  -4.7319 -1.8929 -0.674879 1 0.2 0
  -0.162023 1.82744 -2.32202 1 0.2 0
  -0.384594 2.82689 -3.52683 1 0.2 0
  -1.29219 2.05831 -2.0948 1 0.2 0
  -1.93874 3.0129 -3.17681 1 0.2 0
  -0.0832456 -3.07112 -0.0825312 1 0.2 0
  -0.184532 -4.63358 -0.302705 1 0.2 0
  2.84351 1.21969 -0.748027 1 0.2 0
  4.28518 1.85112 -0.899284 1 0.2 0
  3.24525 0.112649 0.853299 1 0.2 0
  4.72455 0.00703717 1.40148 1 0.2 0
  -1.33731 -1.79526 2.62888 1 0.2 0
  -2.02527 -2.46023 3.88767 1 0.2 0
  -0.374395 -0.159507 -0.105163 1 0.2 0
  -1.16782 -1.20838 0.772528 1 0.2 0
  0.619218 -2.52038 1.74929 1 0.2 0
  1.11602 -3.70082 2.67652 1 0.2 0
  0 1 -1
  2 3 -1
  4 5 -1
  6 7 -1
  8 9 -1
  10 11 -1
  12 13 -1
  14 15 -1
  16 17 -1
  18 19 -1
  20 21 -1
  22 23 -1
  24 25 -1
  26 27 -1
  28 29 -1
  30 31 -1
  32 33 -1
  34 35 -1
  36 37 -1
  38 39 -1
  40 41 -1
  42 43 -1
  44 45 -1
  46 47 -1
  48 49 -1
  50 51 -1
WorldEnd
FrameEnd

