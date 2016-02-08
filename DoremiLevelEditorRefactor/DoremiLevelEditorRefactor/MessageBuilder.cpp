#include "MessageBuilder.hpp"
#include "ApplicationContext.hpp"
#include "NodeHandler.hpp"

namespace DoremiEditor
{
	namespace Plugin
	{
		TransformInfo MessageBuilder::GetTransformData(std::string p_nodeName) 
		{
			try
			{
				TransformInfo o_data;
				MStatus result;
				MString t_nodeNameMString(p_nodeName.c_str());
				MSelectionList t_selectionList;
				MDagPath t_dagPath;
				if (MGlobal::getSelectionListByName(t_nodeNameMString, t_selectionList))
				{
					t_selectionList.getDagPath(0, t_dagPath);
					if (t_dagPath.hasFn(MFn::kTransform))
					{
						MFnTransform t_transform(t_dagPath.node(), &result);

						//Fill Names
						o_data.nodeName = p_nodeName;
						if (t_transform.parent(0).hasFn(MFn::kTransform))
						{
							MFnTransform t_parent(t_transform.parent(0), &result);
							o_data.parentName = t_parent.name().asChar();
						}
						else
						{
							o_data.parentName = "";
						}

						// Fill Matrix data
						MTransformationMatrix t_transformMatrix = t_dagPath.inclusiveMatrix(&result);
						MVector t_translation = t_transformMatrix.getTranslation(MSpace::kTransform, &result);
						double t_rotations[4];
						double t_scale[3];
						t_transformMatrix.getRotationQuaternion(t_rotations[0], t_rotations[1], t_rotations[2], t_rotations[3]);
						t_transformMatrix.getScale(t_scale, MSpace::kTransform);

						o_data.transformData.translation[0] = t_translation.x;
						o_data.transformData.translation[1] = t_translation.y;
						o_data.transformData.translation[2] = t_translation.z;

						o_data.transformData.rotation[0] = t_rotations[0] * -1.0f;
						o_data.transformData.rotation[1] = t_rotations[1] * -1.0f;
						o_data.transformData.rotation[2] = t_rotations[2] * -1.0f;
						o_data.transformData.rotation[3] = t_rotations[3] * -1.0f;

						o_data.transformData.scale[0] = t_scale[0];
						o_data.transformData.scale[1] = t_scale[1];
						o_data.transformData.scale[2] = t_scale[2];

						MPlug t_plug = t_transform.findPlug("drmRender");
						t_plug.getValue(o_data.transformData.attributes.isRendered);
						t_plug = t_transform.findPlug("drmBBox");
						t_plug.getValue(o_data.transformData.attributes.isBBox);
						t_plug = t_transform.findPlug("drmCollidable");
						t_plug.getValue(o_data.transformData.attributes.isCollider);
						t_plug = t_transform.findPlug("drmAIground");
						t_plug.getValue(o_data.transformData.attributes.isAIground);
						t_plug = t_transform.findPlug("drmInteractable");
						t_plug.getValue(o_data.transformData.attributes.isInteractable);
						t_plug = t_transform.findPlug("drmInteractIntevalX");
						t_plug.getValue(o_data.transformData.attributes.interactIntervalX);
						t_plug = t_transform.findPlug("drmInteractIntevalY");
						t_plug.getValue(o_data.transformData.attributes.interactIntervalY);
						t_plug = t_transform.findPlug("drmInteractIntevalZ");
						t_plug.getValue(o_data.transformData.attributes.interactIntervalZ);
						t_plug = t_transform.findPlug("drmSpawner");
						t_plug.getValue(o_data.transformData.attributes.typeSpawner);
						t_plug = t_transform.findPlug("drmCheckPoint");
						t_plug.getValue(o_data.transformData.attributes.typeCheckPoint);
						t_plug = t_transform.findPlug("drmLevelStartEnd");
						t_plug.getValue(o_data.transformData.attributes.typeStartEnd);

						if (ApplicationContext::GetInstance().GetDebugStatus())
						{
							MString t_str1 = " Translation( " + MString() + o_data.transformData.translation[0];
							t_str1 += " , " + MString() + o_data.transformData.translation[1];
							t_str1 += " , " + MString() + o_data.transformData.translation[2] + " )";
							PrintDebug(MString(o_data.nodeName.c_str()) + t_str1);

							MString t_str2 = " Rotation( " + MString() + o_data.transformData.rotation[0];
							t_str2 += " , " + MString() + o_data.transformData.rotation[1];
							t_str2 += " , " + MString() + o_data.transformData.rotation[2];
							t_str2 += " , " + MString() + o_data.transformData.rotation[3] + " )";
							PrintDebug(MString(o_data.nodeName.c_str()) + t_str2);

							MString t_str3 = " Scale( " + MString() + o_data.transformData.scale[0];
							t_str3 += " , " + MString() + o_data.transformData.scale[1];
							t_str3 += " , " + MString() + o_data.transformData.scale[2] + " )";
							PrintDebug(MString(o_data.nodeName.c_str()) + t_str3);
						}
						return o_data;
					}
					else
					{
						PrintError("Incorrect type (not a transform!)");
					}


				}
				return o_data;
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}

		}
		MeshInfo MessageBuilder::GetMeshData(std::string p_nodeName)
		{
			try
			{
				MStatus result;
				MeshInfo o_data;
				
				MString t_nodeNameMString(p_nodeName.c_str());
				MSelectionList t_selectionList;
				MDagPath t_dagPath;

				if (MGlobal::getSelectionListByName(t_nodeNameMString, t_selectionList))
				{
					t_selectionList.getDagPath(0, t_dagPath);
					if (t_dagPath.hasFn(MFn::kMesh))
					{
						MFnMesh t_mesh(t_dagPath.node(), &result);
						//
						// Get Mesh Data
						//
						const float* t_vertexList;
						const float* t_normalsList;
						MFloatArray t_uArray;
						MFloatArray t_vArray;
						MIntArray t_trianglePerPolyCount;
						MIntArray t_triangleVertexIDs;
						MIntArray t_triangleNormalIDs;
						MIntArray t_triangleUVIDs;

						t_vertexList = t_mesh.getRawPoints(&result);
						t_normalsList = t_mesh.getRawNormals(&result);
						t_mesh.getUVs(t_uArray, t_vArray);
						t_mesh.getTriangles(t_trianglePerPolyCount, t_triangleVertexIDs);

						MItMeshPolygon t_polyIterator(t_dagPath.node(), &result);
						if (t_polyIterator.hasValidTriangulation())
						{
							for (t_polyIterator.reset(); !t_polyIterator.isDone(); t_polyIterator.next())
							{
								int t_trianglesThisPoly = 0;
								t_polyIterator.numTriangles(t_trianglesThisPoly);
								int uId0, uId1, uId2;
								if (t_trianglesThisPoly == 1)
								{
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(0, &result));
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(1, &result));
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(2, &result));

									t_polyIterator.getUVIndex(0, uId0);
									t_polyIterator.getUVIndex(1, uId1);
									t_polyIterator.getUVIndex(2, uId2);
									t_triangleUVIDs.append(uId0);
									t_triangleUVIDs.append(uId1);
									t_triangleUVIDs.append(uId2);
								}
								else if (t_trianglesThisPoly == 2)
								{
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(0, &result));
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(1, &result));
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(3, &result));
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(3, &result));
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(1, &result));
									t_triangleNormalIDs.append(t_polyIterator.normalIndex(2, &result));

									t_polyIterator.getUVIndex(0, uId0);
									t_polyIterator.getUVIndex(1, uId1);
									t_polyIterator.getUVIndex(3, uId2);
									t_triangleUVIDs.append(uId0);
									t_triangleUVIDs.append(uId1);
									t_triangleUVIDs.append(uId2);
									t_polyIterator.getUVIndex(3, uId0);
									t_polyIterator.getUVIndex(1, uId1);
									t_polyIterator.getUVIndex(2, uId2);
									t_triangleUVIDs.append(uId0);
									t_triangleUVIDs.append(uId1);
									t_triangleUVIDs.append(uId2);
								}
								else if (t_trianglesThisPoly > 2)
								{
									bool t_firstIteration = true;
									int t_index1, t_index2;
									MIntArray t_vertsThisPoly;
									t_polyIterator.getVertices(t_vertsThisPoly);
									int t_midValue = t_vertsThisPoly.length() - 1;
									for (int i = 0; i < t_trianglesThisPoly; ++i)
									{
										if (t_firstIteration)
										{
											t_triangleNormalIDs.append(t_polyIterator.normalIndex(0, &result));
											t_triangleNormalIDs.append(t_polyIterator.normalIndex(1, &result));
											t_triangleNormalIDs.append(t_polyIterator.normalIndex(t_midValue - 2, &result));

											t_polyIterator.getUVIndex(0, uId0);
											t_polyIterator.getUVIndex(1, uId1);
											t_polyIterator.getUVIndex(t_midValue, uId2);
											t_triangleUVIDs.append(uId0);
											t_triangleUVIDs.append(uId1);
											t_triangleUVIDs.append(uId2);

											t_index1 = 1;
											t_index2 = t_midValue;
											t_firstIteration = false;
										}
										else
										{
											if (t_index2 - 1 >= 0)
											{
												if (t_index2 - 1 != t_midValue)
												{
													t_index2--;
												}
												else if (t_index2 - 2 >= 0)
												{
													t_index2 -= 2;
												}
												else
												{
													t_index2 = t_midValue + 1;
												}
											}
											else
											{
												t_index2 = t_vertsThisPoly.length() - 1;
											}
											t_triangleNormalIDs.append(t_polyIterator.normalIndex(t_index1, &result));
											t_triangleNormalIDs.append(t_polyIterator.normalIndex(t_index2, &result));
											t_triangleNormalIDs.append(t_polyIterator.normalIndex(t_midValue - 2, &result));

											t_polyIterator.getUVIndex(t_index1, uId0);
											t_polyIterator.getUVIndex(t_index1, uId1);
											t_polyIterator.getUVIndex(t_midValue, uId2);
											t_triangleUVIDs.append(uId0);
											t_triangleUVIDs.append(uId1);
											t_triangleUVIDs.append(uId2);
											
											t_index1 = t_index2;
											t_index2 -= 2;

										}
									}

								}
								else
								{
									PrintWarning("No triangles in face!");
								}
							}
						}
						//
						// Get Materials
						//
						std::string t_materialName;
						MObjectArray t_connectedShaders;
						MIntArray t_shaderIDs;
						t_mesh.getConnectedShaders(0, t_connectedShaders, t_shaderIDs);
						MFnDependencyNode t_shaderGroup(t_connectedShaders[0]);
						MPlug t_plug = t_shaderGroup.findPlug("surfaceShader");
						MPlugArray t_connections;
						t_plug.connectedTo(t_connections, true, false);
						int t_shaderID = -1;
						for (size_t i = 0; i < t_connections.length(); ++i)
						{
							if (t_connections[i].node().hasFn(MFn::kLambert))
							{
								t_shaderID = i;
							}
						}
						if (t_shaderID >= 0)
						{
							MFnDependencyNode t_material(t_connections[t_shaderID].node());
							t_materialName = t_material.name().asChar();
						}
						//
						// Get Parent Names
						//
						std::vector<std::string>t_parentNames;
						size_t t_parentCount = t_mesh.parentCount();
						for (std::vector<std::string>::size_type i = 0; i < t_parentCount; ++i)
						{
							if (t_mesh.parent(i).hasFn(MFn::kTransform))
							{
								MFnTransform t_parentTransform(t_mesh.parent(i),&result);
								t_parentNames.push_back(t_parentTransform.name().asChar());
							}
						}
						//
						// Fill output struct
						//
						o_data.transformCount = t_parentCount;
						o_data.transformName = t_parentNames;
						o_data.materialID = 0;
						o_data.materialName = t_materialName;
						o_data.meshID = 0;
						o_data.nodeName = p_nodeName;
						o_data.meshData.vertCount = t_mesh.numVertices();
						o_data.meshData.normalCount = t_mesh.numNormals();
						o_data.meshData.UVCount = t_mesh.numUVs();
						o_data.meshData.vertices = t_vertexList;
						o_data.meshData.normals = t_normalsList;
						o_data.meshData.indCount = t_triangleVertexIDs.length();
						o_data.meshData.triCount = t_trianglePerPolyCount.length();

						o_data.meshData.triPerFace = new int[t_trianglePerPolyCount.length()];
						t_trianglePerPolyCount.get(o_data.meshData.triPerFace);

						o_data.meshData.triIndices = new int[o_data.meshData.indCount];
						t_triangleVertexIDs.get(o_data.meshData.triIndices);

						o_data.meshData.norIndices = new int[t_triangleNormalIDs.length()];
						t_triangleNormalIDs.get(o_data.meshData.norIndices);

						o_data.meshData.UVIndices = new int[t_triangleUVIDs.length()];
						t_triangleUVIDs.get(o_data.meshData.UVIndices);
						
						if (ApplicationContext::GetInstance().GetDebugStatus())
						{
							MString namestr = MString(p_nodeName.c_str()) + " ";
							MString str = "Parents ( ";
							for (int i = 0; i < o_data.transformCount; ++i)
							{
								str += MString(o_data.transformName[i].c_str()) + " ";
							}
							str += ")";
							PrintDebug(namestr + str);
							PrintDebug(namestr + " Material ( " + MString(o_data.materialName.c_str()) + " )");
							MString str2 = "Vert# " + MString() + o_data.meshData.vertCount;
							str2 += " Normals# " + MString() + o_data.meshData.normalCount;
							str2 += " UVs# " + MString() + o_data.meshData.UVCount;
							PrintDebug(namestr + str2);
							
						}
					}
				}

				return o_data;
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}

		}
		CameraInfo MessageBuilder::GetCameraData(std::string p_nodeName)
		{
			try
			{
				MStatus result;
				CameraInfo o_data;
				MString t_nodeNameMString(p_nodeName.c_str());
				MSelectionList t_selectionList;
				MDagPath t_dagPath;
				if (MGlobal::getSelectionListByName(t_nodeNameMString, t_selectionList))
				{
					t_selectionList.getDagPath(0, t_dagPath);
					if (t_dagPath.hasFn(MFn::kCamera))
					{
						MFnCamera t_camera(t_dagPath.node(), &result);
						std::string t_parentName;
						if (t_camera.parent(0).hasFn(MFn::kTransform))
						{
							MFnTransform t_parentTransform(t_camera.parent(0), &result);
							if (!result)
							{
								PrintWarning("Could not find parent node" + GetNameStrings(p_nodeName, ""));
							}
							else
							{
								t_parentName = t_parentTransform.name().asChar();
							}
						}
						
						MFloatMatrix t_projectionMatrix(t_camera.projectionMatrix());
						MPoint t_position = t_camera.eyePoint(MSpace::Space::kPostTransform);
						MFloatVector t_direction = t_camera.viewDirection(MSpace::Space::kPostTransform);
						MFloatVector t_up = t_camera.upDirection(MSpace::Space::kPostTransform);
						MFloatVector t_right = t_camera.rightDirection(MSpace::Space::kPostTransform);
						bool t_isOrtho = t_camera.isOrtho();
						double t_fov = t_camera.verticalFieldOfView();
						//
						// Fill data struct
						//
						o_data.nodeName = p_nodeName;
						o_data.transformName = t_parentName;
						o_data.camData.isOrtho = t_isOrtho;
						if (t_isOrtho)
						{
							o_data.camData.hAngle = t_camera.orthoWidth();
						}
						else
						{
							o_data.camData.hAngle = t_fov;
						}
						for (int i = 0; i < 3; ++i)
						{
							o_data.camData.rightVector[i] = t_right[i];
							o_data.camData.target[i] = t_direction[i];
							o_data.camData.upVector[i] = t_up[i];
						}
						if (ApplicationContext::GetInstance().GetDebugStatus())
						{
							PrintDebug(MString(p_nodeName.c_str())+" Pos(" + MString() + t_position.x + " , " + MString() + t_position.y + " , " + MString() + t_position.z + ")");
							PrintDebug(MString(p_nodeName.c_str()) + " Dir(" + MString() + t_direction.x + " , " + MString() + t_direction.y + " , " + MString() + t_direction.z + ")");
							PrintDebug(MString(p_nodeName.c_str()) + " Up(" + MString() + t_up.x + " , " + MString() + t_up.y + " , " + MString() + t_up.z + ")");
							PrintDebug(MString(p_nodeName.c_str()) + " Orthographic: " + MString() + t_isOrtho);
							PrintDebug(MString(p_nodeName.c_str()) + " Horizontal fov / Orthographic width: " + MString() + o_data.camData.hAngle);
						}

					}
				}
				return o_data;
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}

		}
		LightInfo MessageBuilder::GetLightData(std::string p_nodeName)
		{
			try
			{
				MStatus result;
				LightInfo o_data;
				MString t_nodeNameMString(p_nodeName.c_str());
				MSelectionList t_selectionList;
				MDagPath t_dagPath;
				if (MGlobal::getSelectionListByName(t_nodeNameMString, t_selectionList))
				{
					t_selectionList.getDagPath(0, t_dagPath);
					if (t_dagPath.hasFn(MFn::kLight))
					{
						MFnLight t_baseLight(t_dagPath.node());
						std::string t_parentName;
						if (t_baseLight.parent(0).hasFn(MFn::kTransform))
						{
							MFnTransform t_parentTransform(t_baseLight.parent(0), &result);
							if (!result)
							{
								PrintWarning("Could not find parent node" + GetNameStrings(p_nodeName, ""));
							}
							else
							{
								t_parentName = t_parentTransform.name().asChar();
							}
						}

						MColor t_diffuseColor = t_baseLight.color();
						float t_RGBColor[3];
						t_diffuseColor.get(MColor::MColorType::kRGB, t_RGBColor[0], t_RGBColor[1], t_RGBColor[2]);
						float t_intensity = t_baseLight.intensity();
						//
						// Fill generic data
						//
						o_data.nodeName = p_nodeName;
						o_data.transformName = t_parentName;
						o_data.lightData.intensity = t_intensity;
						std::copy(t_RGBColor, t_RGBColor + 3, o_data.lightData.colorDiffuse);
						
						//
						// Fill Specific data
						//
						if (t_dagPath.hasFn(MFn::kDirectionalLight))
						{
							o_data.lightData.type = 1;
							MFnDirectionalLight t_directionalLight(t_dagPath, &result);
							MFloatVector t_direction(t_baseLight.lightDirection(0, MSpace::kWorld, &result));
							o_data.lightData.direction[0] = t_direction.x;
							o_data.lightData.direction[1] = t_direction.y;
							o_data.lightData.direction[2] = t_direction.z;

						}
						else if (t_dagPath.hasFn(MFn::kSpotLight))
						{
							o_data.lightData.type = 2;
							MFnSpotLight t_spotLight(t_dagPath, &result);
							o_data.lightData.decayType = t_spotLight.decayRate();
							o_data.lightData.coneAngle = t_spotLight.coneAngle();
							o_data.lightData.penumAgle = t_spotLight.penumbraAngle();
							o_data.lightData.dropOff = t_spotLight.dropOff();
						}
						else if (t_dagPath.hasFn(MFn::kPointLight))
						{
							o_data.lightData.type = 3;
							MFnPointLight t_pointLight(t_dagPath, &result);
							o_data.lightData.decayType = t_pointLight.decayRate();
						}


					}
				}
				return o_data;
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}

		}
		MaterialInfo MessageBuilder::GetMaterialData(std::string p_nodeName)
		{
			try
			{
				MStatus result;
				MaterialInfo o_data;
				MString t_nodeNameMString(p_nodeName.c_str());
				MSelectionList t_selectionList;
				MDagPath t_dagPath;
				if (MGlobal::getSelectionListByName(t_nodeNameMString, t_selectionList))
				{
					MObject t_depObj;
					t_selectionList.getDependNode(0, t_depObj);
					MFnDependencyNode t_material(t_depObj, &result);

					float t_diffuse;
					MPlug t_plug = t_material.findPlug("diffuse", &result);
					if (result)
					{
						t_plug.getValue(t_diffuse);
					}
					
					t_plug = t_material.findPlug("color", &result);
					if (result)
					{
						if (t_plug.isConnected())
						{
							MPlugArray t_plugArray;
							t_plug.connectedTo(t_plugArray, true, false);
							for (int i = 0; i < t_plugArray.length(); ++i)
							{
								MFnDependencyNode t_texture(t_plugArray[i].node());
								t_plug = t_texture.findPlug("fileTextureName", &result);
								if (result)
								{

								}
							}
						}
					}
					



				}
				return o_data;
			}
			catch (...)
			{
				const std::string errorMessage = std::string("Cath: " + std::string(__FUNCTION__));
				PrintError(MString() + errorMessage.c_str());
			}

		}
	}
}