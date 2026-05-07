# Galaxies parser program using HyperLeda database.
**for PixInsight AnnotateImage script**

AnnotateImage is a script in Pixinsight that marks and labels the astronomical objects that are present in an astrophotography, mainly DSOs (deep space objects), but also stars, asteroids and planets that may be in the image.
Although AnnotateImage can natively mark objects from PGC HYPERLEDA catalog, the number of PGC objects it displays is relatively low, probably limited to a certain magnitude value.

For instance, in the image of NGC4565 below, only 8 PGC objects are revealed, although the image contains many more.

![AnnotateImageDefault.png](Images/AnnotateImageDefault.png)

If you want more objects to be displayed, you will have to extract the corresponding data from the HyperLeda database.

The GalaxyDBParser program in this repo will select the galaxies from the data downloaded from HyperLeda and create a .csv file that can be used as a custom catalog in the AnnotateImage script.

Before downloading data from HyperLeda, you have to determine the angular radius around the object of interest so that HyperLeda to select the objects in the area defined by the angular radius. In the image above, the object of interest is in the center - NGC4565 (Needle Galaxy). In this particular case the radius around NGC4565 would be half of the image diagonal. The diagonal is roughly 50 arcminutes, so, a radius of 25 arcminutes would cover the entire image. You can determine the agular size in PixInsight by using the AngularDistance script.

Now you can download the data from the database: open the HyperLeda site (http://atlas.obs-hp.fr/hyperleda/), select “Search near a position or name”. The following page will be displayed.
![HL_SearchNearPositionOrName.png](Images/HL_SearchNearPositionOrName.png)
Input the object name in “**_Object designation_**” field, Click on “_**Find coordinates in HyperLeda**_” button. The celestial position (initially empty) will be displayed.
In the “**_Search radius_**” field input the desired radius (25 arcmin in our case) and press the “**_List objects in HyperLeda_**” button.

In the next page, press on the “**_Advanced constraints_**” button.

In the “**_HyperLeda compilations index_**” page, add the “**_PGC_**” column (select the checkbox) to have the PGC numbers in a separate column.
![HL_Index_1.png](Images/HL_Index_1.png)

At the bottom of the page, change “**_Output as_**” from “**_plain HTML_**” to "**_CSV_**", change the “**_separator_**” to “**_other_**”, input “**_;_**” in the separator field and click on the “**_Save to file_**” button.
![HL_Index_2.png](Images/HL_Index_2.png)
You will be asked to save a text file (**HyperLeda\_a000\_nnnnnnnnnn.txt**), this is the file having the objects in the database within the selected radius and around the selected position. The columns of the file are delimited with semicolons. Save the file.

Go to the location of the GalaxyDBParser program and run it with the path of the formerly saved file as the first commandline parameter, the file name as second parameter and the desired diameter of the circle that will mark the object in the annotated image as the third parameter.
If there are few objects in the image you can use a bigger value for the diameter (1, 1.5), but if there are many objects use a smaller value to avoid overlapping.

_.\GalaxyDBParser.exe D:\Downloads\HyperLeda_a000_1774489569.txt 0.6_
![OutputWin.png](Images/OutputWin.png)
The program has added 235 objects into the output file.

The program keeps only the galaxies in the database, discards the other objects and generates the output file: galaxy_data.csv which can be used by the annotate script in PixInsight.

As of version 1.1, the magnitude of the selected objects can be limited so that only what is visible in the image gets annotated, based on the maximal magnitude that can be found in the image.
To do this, run the program without parameters, ignore the message that there are insufficient arguments in the commandline and answer "y" to continue running the program and input the necessary data. The program will ask for the path to the database file saved fom the HyperLeda site, the file name itself, the diameter of the object circles that will be drawn around the objects and, finally, the magnitude limit for the objects in the database. If you do not wish to limit the magnitude, input "_999_" to the last data request.
![OutputWinNoArguments.png](Images\OutputWinNoArguments.png)
Now you can use the output file (galaxy_data.csv) in PixInsight.

Start PixInsight and launch the **_AnnotateImage_** script.
![AnnotateScript.png](Images/AnnotateScript.png)

Leave the **_Messier_**, _**NGC-IC**_ and **_PGC_** layers active. Click on the green "**_+_**" to add a custom layer.

Click on the "**_Custom_**" layer to select it. In the "**_Custom Parameters_**" section, chose the desired parameters of the markers and text (colour, width, font...).
In the "**_Label Text_**" group you can select what text should be displayed next to the circles: Name, Coordinates or Magnitude and, by choosing the appropriate dropbox, where will the text be relative to the circles.

At the "**_Catalog path_**" property, click on the folder icon on the right, navigate and select the output file generated by the GalaxyDBParser program. 

Click on **_"Preview_**" to visualise how will the image look and if everything is OK, click on the "**_OK_**" button. The annotated image will be generated:
![AnnotateImage.png](Images/AnnotateImage.png)
The new PGC objects from the HyperLeda database are added in yellow colour.

This program is based on the HyperLeda database:  http://leda.univ-lyon1.fr
