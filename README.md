# People and Vehicle Counting
This is simple project on count number of vehicles and people using OpenCV and C++

![alt text][Screenshot]

## How it works
<dl>
<dt>1. Read video from file</dt>
<dt>2. Convert frame into grayscale</dt>
<dt>3. Background subtraction between previous frame and recent frame</dt>
<dt>4. Filter (Erode, Dilate) to eliminate noises</dt>
<dt>5. Find contour from background subtraction result</dt>
<dt>6. Count number of vehicle and people</dt>
<dd> Draw a vertical line as count parameter. Then find value of contour's center point. If point pass through line, it will start count. Use contour area to differentiate between vehicles and people
<dt>7. Show count result</dt>
</dl>

[Screenshot]: https://github.com/adif-git/People_Vehicle_Counting/blob/master/Screenshot.png
