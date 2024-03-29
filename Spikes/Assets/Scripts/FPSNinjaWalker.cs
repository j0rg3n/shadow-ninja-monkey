    using UnityEngine;
using System.Collections;

[RequireComponent(typeof(CharacterController))]
public class FPSNinjaWalker : MonoBehaviour 
{
	public AudioSource runAndSlashSound;
	public AudioSource selfRunAndSlashSound;
	
	public string pokeButton = "space";
	public string blockButton = "left shift";
	
	private enum State {Pouncing, Blocking ,ReadyToFight};
	private State state = State.ReadyToFight;
	
	private NinjaPokerAnimation pokerAnimation;
	public string horizontalAxis = "Horizontal";
	public string verticalAxis = "Vertical";
	public string moveButton = "Fire1";
	
	public bool useMouse = true;
	public float pounceSpeed = 16.0f;
	public float speed = 4.0f;
	public float gravity = 20.0f;
	public float pouncingDuration = 0.5f;
	
	public float blockDuration = 0.4f;

	private Vector3 moveDirection = Vector3.zero;
	private Vector3 lastKnownMoveDir = Vector3.zero;
	private bool touchesGround = false;
		
	private float stateResetTime = 0.0f;
	private NinjaBehaviour ninjaBehaviour;
	
	
	public bool IsBlocking()
	{
		return state == State.Blocking;
	}
	public void Start()
	{
		ninjaBehaviour = GetComponent<NinjaBehaviour>();
		pokerAnimation = GetComponentInChildren<NinjaPokerAnimation>();
	}
	
	void FixedUpdate () 
	{
		if(!ninjaBehaviour.AmIMyself())
		{
			// Avoid messing with objects owned by the other player.
			return;
		}

		switch(state)
		{
			case State.Pouncing:
				pounceForwardWithSwordInHand();
				break;
			case State.Blocking:
				standStillBlockingAttacks();
				break;
			case State.ReadyToFight:
				handleUserInput();
				break;
		}
		
		if(stateResetTime < Time.time && state!=State.ReadyToFight)
		{
			state = State.ReadyToFight;
			pokerAnimation.Stand();
		}
	}	
	
	private void handleUserInput()
	{
		if (Input.GetKey(pokeButton))
		{
			pokerAnimation.Poke();
			state = State.Pouncing;
			stateResetTime = Time.time + pouncingDuration;
			networkView.RPC("OnPounce", RPCMode.All);
		}
		else if (Input.GetKey(blockButton))
		{
			pokerAnimation.Block();
			state = State.Blocking;
			stateResetTime = Time.time + blockDuration;
		}
		else
		{
			handleUserInputMovement();
		}
	
	}
	
	[RPC]
	public void OnPounce()
	{
		if (ninjaBehaviour.AmIMyself())
		{
			selfRunAndSlashSound.Play();
		}
		else
		{
			runAndSlashSound.Play();
		}
	}
	
	private void standStillBlockingAttacks()
	{
		
	}
	
	private void pounceForwardWithSwordInHand()
	{
		MoveCharacter(lastKnownMoveDir * pounceSpeed);
	}

	private void handleUserInputMovement()
	{
		if (touchesGround && state != State.Pouncing) 
		{
			if (useMouse)
			{
				if (Input.GetButton(moveButton))
				{
					Camera cam = Camera.main;
					GameObject plane = GameObject.Find("Plane");
					
					Vector3 pos = cam.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, cam.nearClipPlane));
					Vector3 dir = (pos - cam.transform.position).normalized;
					if (cam.orthographic)
					{
						dir = cam.transform.TransformDirection(Vector3.forward);															}
											
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
				//moveDirection = transform.TransformDirection(moveDirection);
				moveDirection *= speed;
			}
		}
		
		MoveCharacter(moveDirection);		
	}
	
	private void MoveCharacter(Vector3 moveDirection)
	{
		CharacterController controller = GetComponent<CharacterController>();
		moveDirection.y = 0;
		if(moveDirection.sqrMagnitude!= 0){
			lastKnownMoveDir = moveDirection;
			lastKnownMoveDir.Normalize();
			controller.transform.rotation = Quaternion.LookRotation (moveDirection, Vector3.up);
		}
		moveDirection.y -= gravity * Time.deltaTime;
		CollisionFlags flags = controller.Move(moveDirection * Time.deltaTime);
		touchesGround = (flags & CollisionFlags.CollidedBelow) != 0;
	}
}