---
ID: 215
post_title: 'Lidar project: Upgrading to Rpilidar A1'
author: admin
post_excerpt: ""
layout: page
permalink: >
  https://gaborszita.net/news/lidar-project-upgrading-to-rpilidar-a1/
published: true
post_date: 2019-11-17 21:56:36
---
<!-- wp:paragraph -->
<p>Due to problems with the neato xv-11 lidar, I am upgrading to the <a href="http://www.slamtec.com/en/lidar/a1">rpilidar a1</a>. It's quite cheap ($100) for the lidar project.</p>
<!-- /wp:paragraph -->

<!-- wp:heading {"level":4} -->
<h4>Why I upgraded?</h4>
<!-- /wp:heading -->

<!-- wp:list {"ordered":true} -->
<ol><li>The neato xv-11 lidar has a really low scan rate, 3hz (=3 full scans per second). This results in a problem, where when the robot moves the corners points are far from each other and it's hard to make them in pairs. If you don't know what corner points I'm talking about, please refer to the <a href="https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5949039/">XY calculating algorithm</a>. The Rpilidar A1 has a 10hz maximum scan rate</li><li>Due to the low scan rate, when turning the walls are not straight. This happens e. g. because I scan one part of the wall. Then I start turning the robot. When the robot turns, the lidar will detect the rest of the wall at another position, which results in a wall which is not straight. Faster scan rates can fix this problems.</li></ol>
<!-- /wp:list -->

<!-- wp:paragraph -->
<p>I also bought a gyroscope sensor, for better corner points matching.</p>
<!-- /wp:paragraph -->

<!-- wp:heading {"level":4} -->
<h4>What's next?</h4>
<!-- /wp:heading -->

<!-- wp:paragraph -->
<p>I will assemble the new lidar on the robot and then re-write the program so it can communicate with the new lidar. Then I will re-write the XY calculating algorithm, so it will get data from the gyroscpe.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>Thanks for your patience for the XY calculating algorithm delay. But this lidar upgrade was necessary to maintain accurate determination of position.</p>
<!-- /wp:paragraph -->