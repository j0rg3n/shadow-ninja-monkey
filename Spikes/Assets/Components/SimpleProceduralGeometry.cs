using UnityEngine;
using System.Collections;
using System;

public class SimpleProceduralGeometry : MonoBehaviour 
{
	void Start() 
	{
		gameObject.AddComponent<MeshFilter>();
		gameObject.AddComponent<MeshRenderer>();

		Mesh mesh = GetComponent<MeshFilter>().mesh;

		Vector3[] vertices = new Vector3[4];
		vertices[0] = new Vector3(0, 0);
		vertices[1] = new Vector3(1, 0);
		vertices[2] = new Vector3(1, 1);
		vertices[3] = new Vector3(0, 1);
		baseVertices = vertices;

		int[] triangles = new int[2 * 3];
		triangles[0 * 3 + 0] = 0;
		triangles[0 * 3 + 1] = 1;
		triangles[0 * 3 + 2] = 2;

		triangles[1 * 3 + 0] = 0;
		triangles[1 * 3 + 1] = 2;
		triangles[1 * 3 + 2] = 3;

		mesh.vertices = (Vector3[]) vertices.Clone();
		mesh.triangles = triangles;
	}
	
	void Update()
	{
		float t = (float)(Time.time * Math.PI);
		const float s = .1f;

		Mesh mesh = GetComponent<MeshFilter>().mesh;
		Vector3[] vertices = new Vector3[baseVertices.Length];
		for (int i = 0; i < mesh.vertices.Length; ++i)
		{
			float a = i * (float) Math.PI / mesh.vertices.Length;
			
			vertices[i] = new Vector3(baseVertices[i].x + (float) Math.Cos(a + t) * s,
				baseVertices[i].y + (float) Math.Sin(a + t) * s);
		}		

		mesh.vertices = vertices;
		mesh.RecalculateNormals();
	}
	
	private Vector3[] baseVertices;
}
