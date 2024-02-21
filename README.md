# AthenaAI-exam

## Preamble
So I'll get "the elephant in the room" out of the way and say that I didn't do what was asked for. Graphics card driver and build chain issues meant UE5 wasn't operating as it should (I have an older model Alienware that was broken; I had to fix it for this tech exam), and I couldn't directly create UE5.3 C++ code. Didn't help that the exam was set up in a "each step building on the last fashion", so when I couldn't do step 3 the rest of the exam seemed unachieveable. The abomination you see before you is my best effort given what I have and because I can't spend 40 hours getting the build environment up to scratch at the moment. C++ code was written seperately, and provides (to varying degrees of success) the functionality requested; some of the steps I've had to get a little creative on. What I've done has been explained here under each step heading. Also note that I could not version control the "Rural Australia" content pack given the 50MB file size limit. I was going to include just the assets I used, but the ground texture (the smallest one) was necessary and exceeded this limit. As a result, you'll have to download this from the UE5 marketplace and store it as described in the Run Notes section at the end. It was actually really fun doing this once I accepted the limitations I had on my home system, and I realised just how much I have to learn about Unreal Engine (there's lots of new features since UE4, too!). While I suspect you would not hire someone off this, if I do somehow get a job with you I can really see myself loving developing my understanding of this software, and it would be great to work with people with experience in it. 


## Step 1
~1km x 1km landscape created in UE5.3, Australian Outback inspired with trees, rocks and bushes.

## Step 2
Actor BP created with CineCamera component attached. Placed in scene.

## Step 3
Could not directly compile C++ code with UE5 due to issues, so see randomTransform.cpp. This is some C++ code that generates a random (x,y,z) location within a bounding area, in addition to random yaw, pitch and roll. To do this in UE5, I would have created a C++ blueprint and manually added a function similar to this, but using the UE5 API. Logical would have remained similar. 

## Step 4
10 second sequencer created.

## Step 5
Since step 3 could not be done properly, the position wasn't randomised every one second. Instead, the CineCamera object is moved along paths a few times during the 10s track length to demonstrate a basic understanding of the Level Sequencer object. To accomplish this task properly, I would have completed step 3, then had the Level Sequencer trigger an event that calls the randomisation function on the actor. 

## Step 6
10s video created in UE5 and uploaded as RuralAUS.avi. Video subject to the constraints of Step 5.

## Step 7
This part was actually overlooked because I jumped to the next section to write C++ again, and simulated the image input. Assuming this object was still running on the Level Sequencer (the step is a little ambiguously worded, since its not clear to what extent we are to "modify the project"; probably best to assume the sequencer is remaining) the render output would be changed to PNG rather than AVI.

## Step 8
Again, couldn't write C++ into UE5. Instead, see imageAccessor.cpp. This simulates an image, and provides the Accessor_ThreadSafe class, which allows flattened images to be stored in a map as a value associated with a key. Images are simulated by, unsurprisingly, the Image class. The Accessor_ThreadSafe object maintains a map size of 10 entries in the code, and removes from the beginning of the map. The thought behind this eviction policy was to remove the oldest images first, and since std::map stores the elements in ascending order of the key, this seemed like a workable solution. An issue with this policy is that not all images are equally "interesting", so if we're interested in detecting something in a frame, this frame is just as likely to be removed as any other. Depending on the requirement, it might be better to preprocess the image in some way (redimentary image analysis, edge detection) and assign an "interest" variable. Lowest interest images can be removed. The other issue with my particular implementation is that threads are all created in one go, which means the order that images come in are not guaranteed to be consecutive. This means when we evict items from the beginning of the map, this gives non-deterministic results.

## Step 9
Tested code operates as expected and monitored map size in imageAccessor.cpp.

## Step 10
Done under step 8 and in README.md

## Run Notes
Need to download the "Rural Australia" asset pack from Epic Games Marketplace and add it to Content/RuralAustralia. Cannot store asset pack in Git due to a number of files exceeding 50MB.