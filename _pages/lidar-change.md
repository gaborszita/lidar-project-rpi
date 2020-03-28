---
ID: 246
post_title: >
  Why did I change the lidar from the
  neato XV-11 to the rplidar A1?
author: admin
post_excerpt: ""
layout: page
permalink: >
  https://gaborszita.net/articles/lidar-change/
published: true
post_date: 2020-01-03 22:45:15
---
<!-- wp:paragraph -->
<p>If you were reading my posts you must've seen, that I upgraded from the Neato XV-11 lidar to the rplidar A1 in the <a href="https://gaborszita.net/my-projects/lidar-project/">lidar project</a>.</p>
<!-- /wp:paragraph -->

<!-- wp:table -->
<figure class="wp-block-table"><table class=""><tbody><tr><td class="has-text-align-center" data-align="center"><img src="https://gaborszita.net/wp-content/uploads/2020/01/download-1.jpg" style="width: 200px;"><br>Neato XV-11 lidar</td><td class="has-text-align-center" data-align="center"><img src="https://gaborszita.net/wp-content/uploads/2020/01/61WePzXTCLL._AC_SY355_.jpg" style="width: 200px;"><br>Rplidar A1</td></tr></tbody></table></figure>
<!-- /wp:table -->

<!-- wp:paragraph -->
<p>But why did I upgrade?</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>First of all, I had issues with Neato XV-11 lidar's scan rate. It spins at about 180 rpm, which results in a scan rate of 3hz. In contrast the rplidar A1 spins at about 420 rpm, a scan rate 7hz.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>The first problem I encountered is that it's hard to match the corner feature points (needed to the <a href="https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5949039/">algorithm</a> which calculate the robot's position) when there's a that low scan rate. When the robot moves, in two consecutive scans the points will be farther from each other and so will be the corner feature points. The farther the corner feature points are from each other, the more challenging to match them.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>The other issue is that when the robot turns the straight walls will get curved. This happens because the lidar is spinning very slowly, so as the robot turns, the lidar will see different parts of the wall in different positions.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>As you can see, all these issues or because the slow scan rate (which is caused by the slow spin speed). The rplidar A1 solved this problem, because it has a 7hz scan rate instead of a 3hz scan rate. I choosed this lidar, because it is relatively cheap, around $100.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>The SDK of the rplidar A1 also helped a lot. It was really easy to inject this lidadr in the existing code of the program. The challenging part of it was that is had lots of classes and namespaces (I'm not that familiar with them). The other issue I had to overcome, is that this lidar scans doesn't scan once every degree, like the XV-11. It scans at a 8000 sample rate, which means it scans every 1/8000 of a second. That way, not all degree receive a scan. But I overcame that issue by digitally founding out what distance is at that degree.</p>
<!-- /wp:paragraph -->