# vicar_parser
Parser of the NASA JPL image file formar VICAR. 

![image description](resources/demo_voyager.png)

## Header

Refered as the labels section in the documentation(https://planetarydata.jpl.nasa.gov/img/w10n/voyager/VGISS_0030/DOCUMENT/VICAR.HTML), it containts system labels, porperty labels and history labels.

- **System labels**: Describe the format of the image and how to access it. 
- **Property labels**: Describe properties of the image in the image domain.
- **History labels**: Describe the processing history of the image.

### Example 
```
LBLSIZE=2072            FORMAT='HALF'  TYPE='IMAGE'  BUFSIZ=20480  DIM=3  EOL=0  RECSIZE=2072  ORG='BSQ'  NL=1024
NS=1024  NB=1  N1=1024  N2=1024  N3=1  N4=0  NBB=24  NLB=1  HOST='VAX-VMS'  INTFMT='LOW'  REALFMT='VAX'  BHOST='SUN-4'
BINTFMT='HIGH'  BREALFMT='IEEE'  BLTYPE='CASSINI-ISS'  PROPERTY='CASSINI-ISS'  MISSION_NAME='CASSINI'    INSTRUMENT_ID='ISSNA'
IMAGE_NUMBER=110366  IMAGE_TIME='1996-065T20:42:44.000'  SOFTWARE_VERSION_ID='GSE-V4.0-10-95'  INSTRUMENT_MODE_ID='FULL'
FILTER1_NAME='CL1'  FILTER2_NAME='CL2'  EXPOSURE_DURATION=0.0  GAIN_MODE_ID='100K'  ENCODING_TYPE='NOTCOMP'
CONVERSION_TYPE='12BIT'  DETECTOR_TEMPERATURE=-89.82  OPTICS_TEMPERATURE=32.62  FILTER_TEMPERATURE=-999.00
 LIGHT_FLOOD_STATE_FLAG='ON'  ANTIBLOOMING_STATE_FLAG='OFF'  CALIB_LAMP_STATE_FLAG='OFF'  COMPRESSION_PARAMETER_VALUE=0
GROUP_BLOCKS=0  QUANTIZATION_FACTOR_INDEX=0  ALGORITHM=0  BLOCK_TYPE=0  OFFSET=303  DARK_CURRENT=282  COMPRESSION_RATIO=-999.000
TARGET_NAME='NAC_FOCUS'  OBSERVATION_ID='FOCUS_TEST_200'    RADIANCE=-999.0  MISSING_LINES=0  MISSION_PHASE_TYPE='NAC_FL_T/V'
ILLUMINANT='250W_TUN_NAC_COL'  TASK='ISS_EGSE'  USER='mad'  DAT_TIM='Tue Mar  5 20:43:31 1996'  COMMENT='+3O C
POSITION        .020'  TASK='COPY'  USER='DDS812'  DAT_TIM='Wed Apr 30 08:53:39 1997'
```

## Body

This section contains the data, contains both the binary header and the binary prefixes, both of which are poorly documented. However, this are not entirely necessary for simple image viewing. The file can be read by records, which, in the case of an image, contains one row of pixels.

## EOL

TODO

> **Note**: This is a WIP
