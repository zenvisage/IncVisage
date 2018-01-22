# IncVisage: A Tool for Incremental Generation of Visualizations  
IncVisage is a tool that facilitates faster analysis of large scale datasets by incrementally revealing its features thus allowing users to gain insights quickly (within seconds) and terminate early. To do so, we introduce a new paradigm called "incrementally improving visualizations" where the underlying algorithms progressively draw samples from the dataset and reveal insights when certain guarantees are satisifed. We specifically focus on trendlines and heatmaps as these are the most common mechanisms to interact with and extract insights from time series datasets. The following figure shows how users can utilize IncVisage to visualize incremntal generation of trendlines and heatmaps. 

![Alt text](/images/inc.PNG?raw=true "Incrementally Improving Visualizations")

### Version
The current version is still in beta mode. We are working on publishing our first stable version.

### Features
IncVisage implements incrementally improving visualization generation algorithms for trendlines and heatmaps.

* Incvisage reveals features of a visualization in the order of prominence (important features first). As samples are being drawn, it reveals features only when certain error guarantees are met. Therefore, users can draw conclusions and maked decisons in a guranted fashion. 

* Incvisage provides an options menu select attributes of a dataset that is tanslated to an aggregate query at the back end.

* Incvisage provides an anamation pane to play, pause and reset the visualization generations at any point of type. Users can also take snapshots of a iteration and compare it with newer iterations when making decisions.   

![Alt text](/images/front.PNG?raw=true "IncVisage Front End")

### Additional Readings
* Our VLDB'17 paper describing the key techniques, solutions and evaluations of IncVisage can be found [here][incvisage-vldb].

### Required Tools
* EWAHboolArray. Git repository [here][ewah-repo]
* yaml-cpp. Git repository [here][yaml-cpp]

### Installation Instructions
For now, we IncVisage installation instructions for Linux environment only. 

* Clone the incvisage repository. (Alternatively, you can download the source as a zip.)
     
        git clone https://github.com/zenvisage/incvisage/incvisage.git
     
* Clone the EWAHboolArray repository at /needletail-code-base

* Clone the yaml-cpp repository at /needletail-code-base

* Create a binding.gyp file under /needletail-code-base and add required directories, and libraries. We have added an example binding.gyp file with required directories and libraries. Feel free to reuse that file.

* Set the following in line 48 of /needletail-code-base/yaml-cpp/CMakeLists.txt
		
			option(BUILD_SHARED_LIBS "Build Shared Libraries" ON) 

* Build code. Inside the /needletail-code-base folder using [node-gyp][nd-gyp]:
 
            node-gyp configure build
            
* Run the tool. Inside the root folder,

			node app.js

* Launch `http://localhost:8080/` (preferably in Chrome). 

### Dataset Requirements

Currently, IncVisage only accepts .sdb dataset file. There are python scripts available to transforms .csv files to .sdb files in /needletail-code-base/scripts.

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [incvisage-vldb]: http://data-people.cs.illinois.edu/papers/incvisage-paper.pdf
   [ewah-repo]: https://github.com/lemire/EWAHBoolArray
   [yaml-cpp]: https://github.com/jbeder/yaml-cpp
   [nd-gyp]: https://nodeaddons.com/c-processing-from-node-js/
  
