Unreal Engine 5 Generalist – Practical Programming Test
Overview

Role: Game Developer (Generalist)

Type: Practical programming project

Engine version: Unreal Engine 5.6 or newer

Primary language: C++ (Blueprints only for animation, UI, or data; core logic must be in C++)

Estimated duration: 1–2 days

Target platform: PC Windows x64

Deliverable: Compressed project folder + short write-up in PDF or any other format
Task Description

Create a new Unreal Engine level that walks the player through a short sequence of actions:

Camera transitions (third-person cinematic intro → first-person view)
Basic first-person movement + mouse look
Mission system with visual indicators (UI notifications + indicators/markers)
Interactive door locked with a 4-digit numeric password
Final victory animation after the door is opened

All gameplay logic and mission behavior must be written in C++; UI (widgets) may be mixed.

You may use any existing animations, models, sounds, and textures, but you may not copy ready-made C++ code or entire UI widgets from external sources.

All features must be responsive.
Detailed Functional Requirements (Checklist)

The candidate must implement all of the following. The evaluator will mark each item as Pass/Fail.
3.1 Camera & Level Start

[ ] Level loads with a cinematic camera showing a slow, smooth move toward the player character.

[ ] At the end of the move, an "eyes closing" effect is displayed (e.g. fade to black or vignette + blend).

[ ] After the blend, the view switches to first-person (standard FPS camera).

[ ] FOV is set to a comfortable value (e.g. 90°).
3.2 First-Person Controls

[ ] Standard mouse look (pitch and yaw) with appropriate clamping.

[ ] Movement keys: W/A/S/D for forward/left/backward/right.
3.3 Mission System & Indicators

[ ] Mission system implemented in C++.

[ ] First mission displayed at start: "Go to the mission point."

[ ] World indicator (icon/any sign) points to the target location.

[ ] UI notification with smooth animation (fade in + fade out) appears on mission activate/change/complete.

[ ] First mission completes when the player reaches a trigger volume near the door.
3.4 Door & Password Lock

[ ] Door is an interactive mesh.

[ ] After the first mission completes, the objective updates to: "Open the door — enter the 4-digit password."

[ ] Player can interact with the door (press a key, e.g. E) to open the password widget.

[ ] Widget displays: 4 input fields (digits only) and a way to change each digit (arrow keys or direct input).

[ ] Wrong password → error message displayed (disappears after 2 seconds); door remains locked.

[ ] Correct password (fixed 4-digit code, e.g. 1234) → widget closes with smooth animation, door opens (smooth rotation/sliding/scaling).

[ ] After the door opens, the mission is marked as complete.
3.5 Final Victory Animation

[ ] Only after the door mission is complete, the player character plays a celebration animation (raises hands, jumps, dances, etc.).

[ ] Animation triggers exactly once at that moment.
Technical Constraints & Rules

C++ only for logic: mission logic, input handling, widget creation/binding, gameplay must be in C++. Blueprints are allowed only as data containers (animation references), UI/UX, animations, visual elements, or level dressing.
UI must be C++: password widget and mission notification widget from C++ (subclass of UUserWidget). Widget layouts may be designed in UMG, but all binding/interaction logic must be in C++.
Assets: any animations/sounds/meshes/materials you already have or create temporarily. Do not copy entire ready-made Unreal example projects for the core logic.
Project type: project must be C++ (have a .uproject file with "Modules" and a .Build.cs).
No additional plugins or third-party libraries: only standard Unreal Engine 5 modules.
Documentation: code must be commented (clear inline comments). Provide a short document (PDF or any format) explaining the architecture and assumptions.

Deliverables

Full project folder (archived), excluding: Intermediate/, Saved/, DerivedDataCache/, Binaries/, .vs/
Readme.pdf (max 2 pages) containing:


Brief description of the main C++ classes and their roles
Known issues or unfinished features
Time spent on the test
How to run (e.g. "Open TestLevel and press Play")


Short video (optional but recommended) showing the test project in action (max 2 minutes)

Evaluation Criteria

Feature completion: all checklist items work, no major bugs.
Code readability and documentation.
Optimization and performance.
Architecture and UE5 best practices.
Execution speed.

Submission

Submit via [your chosen platform]. Deadline: [specify date].
