using UnityEngine;
using System.Collections;

[RequireComponent(typeof(CharacterController))]
public class FPSNinjaWalker : MonoBehaviour 
{
	public string horizontalAxis = "Horizontal";
	public string verticalAxis = "Vertical";
	public string moveButton = "Fire1";
	
	public bool useMouse = true;
	
	public float speed = 6.0f;
	public float gravity = 20.0f;

	private Vector3 moveDirection = Vector3.zero;
	private bool grounded = false;
	
	private float lastDropTime = 0.0f;
	
	void FixedUpdate () 
	{
		NetworkView networkView = GetComponent<NetworkView>();
		if (networkView != null && !networkView.isMine)
		{
			// Avoid messing with objects owned by the other player.
			return;
		}

		if (grounded) 
		{
			// We are grounded, so recalculate movedirection directly from axes
			if (useMouse)
			{
				if (Input.GetButton(moveButton))
				{
					Camera cam = Camera.main;
					GameObject plane = GameObject.Find("Plane");
					
					Vector3 pos = cam.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, cam.nearClipPlane));
					Vector3 dir = (pos - cam.transform.position).normalized;
					
					Vector3 planeNormal = plane.transform.TransformDirection(Vector3.up);
					float d = -Vector3.Dot(planeNormal, plane.transform.position);
					float td = Vector3.Dot(planeNormal, dir);
					if (td != 0)
					{
						float t = -(Vector3.Dot(planeNormal, pos) + d) / td;

						Vector3 mouseOnPlane = pos + dir * t;
						
						moveDirection = (mouseOnPlane - transform.position).normalized * speed;
					}
				}
				else
				{
						moveDirection = Vector3.zero;
				}
			}
			else
			{
				moveDirection = new Vector3(Input.GetAxis(horizontalAxis), 0, Input.GetAxis(verticalAxis));
				moveDirection = transform.TransformDirection(moveDirection);
				moveDirection *= speed;
			}
		}

		// Apply gravity
		moveDirection.y -= gravity * Time.deltaTime;
		
		// Move the controller
		CharacterController controller = GetComponent<CharacterController>();
		CollisionFlags flags = controller.Move(moveDirection * Time.deltaTime);
		grounded = (flags & CollisionFlags.CollidedBelow) != 0;
	}
}