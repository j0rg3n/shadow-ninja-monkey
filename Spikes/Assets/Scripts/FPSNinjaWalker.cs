using UnityEngine;
using System.Collections;

[RequireComponent(typeof(CharacterController))]
public class FPSNinjaWalker : MonoBehaviour 
{
	public string horizontalAxis = "Horizontal";
	public string verticalAxis = "Vertical";
	
	public bool useMouse = true;
	
	public float speed = 6.0f;
	public float gravity = 20.0f;

	private Vector3 moveDirection = Vector3.zero;
	private bool grounded = false;
	
	private float lastDropTime = 0.0f;
	
	private Vector3 mouseOnPlane;

	void OnDrawGizmos()
	{
		Gizmos.DrawSphere(mouseOnPlane, 1.0f);
	}	
	
	void FixedUpdate () 
	{
		if (grounded) 
		{
			// We are grounded, so recalculate movedirection directly from axes
			if (useMouse)
			{
				//if (Input.mouseOverWindow)
				{
					Camera cam = Camera.main;
					Vector3 pos = cam.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, cam.nearClipPlane));
					Vector3 dir = cam.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, cam.nearClipPlane - 1)) - pos;
					GameObject plane = GameObject.Find("Plane");
					Vector3 planeNormal = plane.transform.TransformDirection(Vector3.up);
					float d = -Vector3.Dot(planeNormal, plane.transform.position);
					
					float planeDistance = Vector3.Dot(pos, planeNormal) + d;
					mouseOnPlane = pos + dir.normalized * planeDistance;
					//moveDirection = new Vector3(Input);
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