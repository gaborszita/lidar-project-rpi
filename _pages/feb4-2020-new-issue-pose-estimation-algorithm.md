---
ID: 274
post_title: 'What&#8217;s the new issue in the pose estimation algorithm?'
author: admin
post_excerpt: ""
layout: page
permalink: >
  https://gaborszita.net/articles/feb4-2020-new-issue-pose-estimation-algorithm/
published: true
post_date: 2020-02-06 01:09:57
---
<!-- wp:paragraph -->
<p>The new issue in the algorithm is doesn't calculate the correct movement coordinates. It calculates the direction and increases/decreases the x and y coordinates, but not the correct amount.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>I think this issue is because lidar noise. Sometimes it detects points as corners which are just noise, but more often the points which are real corners just move around a lot. When it matches the corners and calculates the displacement, it includes the noise in the calculation.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p><strong>So how will I fix it?</strong></p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>I plan to match find the corner pairs for all consecutive readings, but will not run the ICP algorithm (which calculates displacement). I will match points, which were matched in the previous readings and using that matches after about 5-6 readings I will run the ICP based on the first and the last reading corner point matches. I know this is hard to understand, so here's the flowchart algorithm:</p>
<!-- /wp:paragraph -->

<!-- wp:image {"id":276,"sizeSlug":"large"} -->
<figure class="wp-block-image size-large"><img src="https://gaborszita.net/wp-content/uploads/2020/02/pose-algorithm-flowchart-v2.jpg" alt="" class="wp-image-276"/></figure>
<!-- /wp:image -->