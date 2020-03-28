---
ID: 71
post_title: My homemade safe
author: admin
post_excerpt: ""
layout: page
permalink: >
  https://gaborszita.net/my-projects/my-homemade-safe/
published: true
post_date: 2019-08-20 00:05:35
---
<!-- wp:paragraph -->
<p>I made a homemade safe.</p>
<!-- /wp:paragraph -->

<!-- wp:html -->
<iframe width="100%" height="315" src="https://www.youtube.com/embed/F0gQcQDLtGA" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
<!-- /wp:html -->

<!-- wp:paragraph -->
<p>Web server also on it:</p>
<!-- /wp:paragraph -->

<!-- wp:image {"id":75} -->
<figure class="wp-block-image"><img src="https://gaborszita.net/wp-content/uploads/2019/08/web1-1.jpg" alt="" class="wp-image-75"/></figure>
<!-- /wp:image -->

<!-- wp:image {"id":76} -->
<figure class="wp-block-image"><img src="https://gaborszita.net/wp-content/uploads/2019/08/web2-1.jpg" alt="" class="wp-image-76"/></figure>
<!-- /wp:image -->

<!-- wp:paragraph -->
<p>Intructions</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>1. <strong>Hardware:</strong></p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>First, buy a raspberry pi($35), an Adafruit CharLCD plate, an electric lock,an NPN transistor and an adapter, that has both 12V and 5V output. Then build a case(could be wood, like mine or anything you want.) For setting up the raspberry pi see <a href="https://www.raspberrypi.org/help/noobs-setup/2/">this tutorial</a>. Enabling vnc and ssh is also helpful.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p> 2. <strong>Building it:</strong></p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>First, buy a raspberry pi($35), an Adafruit CharLCD plate, an electric lock,an NPN transistor and an adapter, that has both 12V and 5V output. Then build a case(could be wood, like mine or anything you want.) For setting up the raspberry pi see <a href="https://www.raspberrypi.org/help/noobs-setup/2/">this tutorial</a>. Enabling vnc and ssh is also helpful.</p>
<!-- /wp:paragraph -->

<!-- wp:image {"id":82} -->
<figure class="wp-block-image"><img src="https://gaborszita.net/wp-content/uploads/2019/08/safeoff1little-1-576x1024.jpg" alt="" class="wp-image-82"/></figure>
<!-- /wp:image -->

<!-- wp:image {"id":83} -->
<figure class="wp-block-image"><img src="https://gaborszita.net/wp-content/uploads/2019/08/safeoff2little-1-1024x576.jpg" alt="" class="wp-image-83"/></figure>
<!-- /wp:image -->

<!-- wp:paragraph -->
<p>  3. <strong>Wiring:</strong></p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>Connect the wires as shown. You can connect the NPN to another pin, but then you will need to change the code.</p>
<!-- /wp:paragraph -->

<!-- wp:image {"id":85} -->
<figure class="wp-block-image"><img src="https://gaborszita.net/wp-content/uploads/2019/08/schematics-1.gif" alt="" class="wp-image-85"/></figure>
<!-- /wp:image -->

<!-- wp:paragraph -->
<p> 4. <strong>Software:</strong></p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>Configure the Adafruit CharLCD plate using <a href="https://learn.adafruit.com/character-lcd-with-raspberry-pi-or-beaglebone-black/usage">this tutorial</a>. Download the <a href="https://github.com/gaborszita/safe/blob/master/safe.py">safe.py</a> and the <a href="https://github.com/gaborszita/safe/blob/master/code.txt"> code.txt</a> file and paste it in /home/pi/safe (you can put it anywhere, but then you will need to change the code.) Run it. It work's! Button up and down change the value of the digits, and the left and right set's what digit to change. And it start's to display the time if it's inactive for 2 minutes. To start it every time on startup edit the /etc/rc.local and type <code>sudo python /path/to/safe.py/file</code> before the 'exit0'.</p>
<!-- /wp:paragraph -->

<!-- wp:paragraph -->
<p>If you want also web server on it intall the apache2 web server and configure python script running using <a href="https://www.raspberrypi.org/forums/viewtopic.php?t=155229">this tutorial</a>. Paste <a href="https://github.com/gaborszita/safe/blob/master/index.html">this</a> file in /var/www/html. Download the <a href="https://github.com/gaborszita/safe/blob/master/safeweb.py">safeweb.py</a> file and paste it in /usr/lib/cgi-bin. Make it executable, so type in the terminal window<code> sudo chmod +x/usr/lib/cgi-bin/swafeweb.py</code>. Restart the web server using <code>sudo service apache2 restart</code>. Make the code.txt writeable by anyone, so code.txt →right click →Properties →Permissions →Change content →Anyone. Reboot rpi. Enter the raspberry pi's IP adress in your browser. Login with password 'mypassword'. It should now go to a page like the 2. image. You can change the password by editing the safeweb.py at the 11. line.</p>
<!-- /wp:paragraph -->